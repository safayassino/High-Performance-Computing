#!/bin/bash

# Configuration
CORES=64
MATRIX_SIZES=($(seq 2000 1000 20000))
PRECISIONS=("float" "double")
THREAD_BINDINGS=("spread" "close")
LIBRARIES=("openblas" "blis")

# BLIS path
BLIS_PATH="/u/dssc/syassin/myblis"

echo "Starting benchmarks on EPYC with ${CORES} cores..."

# Initialize CSV files with headers
echo "M,N,K,Time(s),GFLOPS,Precision,Binding,Cores" > openblas.csv
echo "M,N,K,Time(s),GFLOPS,Precision,Binding,Cores" > blis.csv

# Loop through libraries
for LIBRARY in "${LIBRARIES[@]}"; do
    
    echo "========================================"
    echo "Testing library: ${LIBRARY}"
    echo "========================================"
    
    # Load appropriate module/library
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
        
        # Compile for current precision - CHANGED: your_code.c -> gemm.c
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
            
            # Run for each matrix size (square matrices: M=N=K)
            for SIZE in "${MATRIX_SIZES[@]}"; do
                echo "  Testing size: ${SIZE}x${SIZE}x${SIZE}"
                ./gemm_test ${SIZE} ${SIZE} ${SIZE}
            done
            
            echo ""
        done
        
    done
done

# Cleanup
rm -f gemm_test

echo "All benchmarks completed!"
echo "Results saved in:"
echo "  - openblas.csv"
echo "  - blis.csv"