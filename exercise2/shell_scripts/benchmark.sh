#!/bin/bash
# EPYC

CORES=64
MATRIX_SIZES=($(seq 2000 1000 20000))
PRECISIONS=("float" "double")
THREAD_BINDINGS=("spread" "close")
LIBRARIES=("openblas" "blis")

BLIS_PATH="/u/dssc/syassin/myblis"

echo "Starting benchmarks on EPYC with ${CORES} cores..."

echo "M,N,K,Time(s),GFLOPS,Precision,Binding,Cores" > openblas.csv
echo "M,N,K,Time(s),GFLOPS,Precision,Binding,Cores" > blis.csv


for LIBRARY in "${LIBRARIES[@]}"; do
    
    if [ "$LIBRARY" == "openblas" ]; then
        module load openBLAS/0.3.29-omp
        LIB_FLAG="-DOPENBLAS"
        LINK_FLAGS="-lopenblas -lm -lpthread"
        INCLUDE_FLAGS=""
    else
        module purge
        LIB_FLAG="-DBLIS"
        LINK_FLAGS="-lblis -lm -lpthread"
        INCLUDE_FLAGS="-I${BLIS_PATH}/include/blis -L${BLIS_PATH}/lib"
        export LD_LIBRARY_PATH=${BLIS_PATH}/lib:$LD_LIBRARY_PATH
    fi
    
    # Loop through precisions
    for PRECISION in "${PRECISIONS[@]}"; do
        
        echo "Compiling for ${LIBRARY} with ${PRECISION} precision..."
        
        if [ "$PRECISION" == "float" ]; then
            gcc -O3 ${LIB_FLAG} -DUSE_FLOAT ${INCLUDE_FLAGS} gemm.c -o gemm_test ${LINK_FLAGS}
        else
            gcc -O3 ${LIB_FLAG} -DUSE_DOUBLE ${INCLUDE_FLAGS} gemm.c -o gemm_test ${LINK_FLAGS}
        fi
        
        if [ $? -ne 0 ]; then
            echo "ERROR: Compilation failed for ${LIBRARY} ${PRECISION}"
            continue
        fi
        
        # Loop through thread bindings
        for BINDING in "${THREAD_BINDINGS[@]}"; do
            
            echo "Running: Library=${LIBRARY}, Precision=${PRECISION}, Binding=${BINDING}"
            
            # Set OpenMP environment variables
            export OMP_NUM_THREADS=${CORES}
            
            if [ "$BINDING" == "spread" ]; then
                export OMP_PROC_BIND=spread
                export OMP_PLACES=threads
            else
                export OMP_PROC_BIND=close
                export OMP_PLACES=threads
            fi
            
            for SIZE in "${MATRIX_SIZES[@]}"; do
                echo "  Testing size: ${SIZE}x${SIZE}x${SIZE}"
                ./gemm_test ${SIZE} ${SIZE} ${SIZE}
            done
            
            echo ""
        done
        
    done
done

rm -f gemm_test