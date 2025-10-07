#!/bin/bash
#SBATCH --job-name=gol_omp_scale
#SBATCH --output=omp_scale_%j.out
#SBATCH --error=omp_scale_%j.err
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=12
#SBATCH --time=02:00:00
#SBATCH --partition=THIN
#SBATCH --exclusive

cd $SLURM_SUBMIT_DIR

ROWS=20000
COLS=20000
STEPS=50
MODE=1
CSV_FILE="results_openmp.csv"
GRID_FILE="grid_20k.pgm" 

if [ ! -f ${CSV_FILE} ]; then
    echo "study,rows,cols,steps,mode,mpi_procs,omp_threads,time" > ${CSV_FILE}
fi

if [ ! -f ${GRID_FILE} ]; then
    ./gol_serial -i -k ${ROWS},${COLS} -f ${GRID_FILE}
fi

for threads in 1 2 4 6 8 10 12; do
    export OMP_NUM_THREADS=${threads}
    ./gol_omp -r -f ${GRID_FILE} -e ${MODE} -n ${STEPS}
done