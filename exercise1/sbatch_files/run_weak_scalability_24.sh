#!/bin/bash
#SBATCH --job-name=gol_weak_48
#SBATCH --output=weak_%j.out
#SBATCH --error=weak_%j.err
#SBATCH --nodes=2
#SBATCH --ntasks-per-node=12      
#SBATCH --cpus-per-task=1         
#SBATCH --time=02:00:00
#SBATCH --partition=THIN

rm grid_*.pgm
module load openMPI/4.1.6
cd $SLURM_SUBMIT_DIR

BASE_ROWS=8000
BASE_COLS=8000
STEPS=50
MODE=1

export OMP_NUM_THREADS=1           # here we disable OpenMP

echo "study,rows,cols,steps,mode,mpi_procs,omp_threads,time" > results_weak.csv

for nprocs in 1 2 4 8 12 16 24; do
    ROWS=${BASE_ROWS}
    COLS=$((BASE_COLS * nprocs))
    GRID_FILE="grid_weak_${nprocs}.pgm"
    
    echo "=========================================="
    echo "Test ${nprocs}: Grid ${ROWS}x${COLS}"
    echo "=========================================="
    
    if [ ! -f ${GRID_FILE} ]; then
        echo "Creating ${GRID_FILE}..."
        ./gol_serial -i -k ${ROWS},${COLS} -f ${GRID_FILE}
    fi
    
    echo "Running with ${nprocs} MPI processes"
    mpirun -np ${nprocs} ./gol_mpi -r -f ${GRID_FILE} -e ${MODE} -n ${STEPS}
    
    tail -n 1 results_mpi.csv >> results_weak.csv
done