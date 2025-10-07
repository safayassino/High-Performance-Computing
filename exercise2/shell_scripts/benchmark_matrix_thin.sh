#!/bin/bash

MATRIX_SIZE=10000
CORE_COUNTS=(1 2 4 6 8 10 12) # THIN
PRECISIONS=("float" "double")
THREAD_BINDINGS=("spread" "close")
LIBRARIES=("openblas" "blis")

BLIS_PATH="/u/dssc/syassin/myblis"

echo "Starting core scaling benchmarks on THIN with matrix size ${MATRIX_SIZE}x${MATRIX_SIZE}x${MATRIX_SIZE}..."

echo "M,N,K,Time(s),GFLOPS,Precision,Binding,Cores" > openblas_matrix_thin.csv
echo "M,N,K,Time(s),GFLOPS,Precision,Binding,Cores" > blis_matrix_thin.csv

#here we loop across the configuration conditions
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
        
        # Compile for current precision
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
            
            if [ "$BINDING" == "spread" ]; then
                export OMP_PROC_BIND=spread
                export OMP_PLACES=threads
            else
                export OMP_PROC_BIND=close
                export OMP_PLACES=threads
            fi
            
            # Loop through different core counts
            for CORES in "${CORE_COUNTS[@]}"; do
                echo "  Testing with ${CORES} cores"
                
                export OMP_NUM_THREADS=${CORES}
                
                ./gemm_test ${MATRIX_SIZE} ${MATRIX_SIZE} ${MATRIX_SIZE}
            done
            
            echo ""
        done
        
    done
done


rm -f gemm_test
