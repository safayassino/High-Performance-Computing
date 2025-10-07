#!/bin/bash
#SBATCH --job-name=gol_omp_epyc
#SBATCH --output=omp_epyc_%j.out
#SBATCH --error=omp_epyc_%j.err
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=64
#SBATCH --time=00:30:00
#SBATCH --partition=EPYC
#SBATCH --exclusive

cd $HOME/projects/High-Performance-Computing/exercise1
rm -f ${GRID_FILE}

ROWS=20000
COLS=20000
STEPS=50
MODE=1
CSV_FILE="results_openmp_epyc.csv"
GRID_FILE="grid_20k.pgm"

if [ ! -f ${CSV_FILE} ]; then
    echo "study,rows,cols,steps,mode,mpi_procs,omp_threads,time" > ${CSV_FILE}
fi

if [ ! -f ${GRID_FILE} ]; then
    ./gol_serial -i -k ${ROWS},${COLS} -f ${GRID_FILE}
fi

export OMP_NUM_THREADS=1
./gol_omp -r -f ${GRID_FILE} -e ${MODE} -n ${STEPS}

for threads in 2 4 8 16 24 32 40 48 56 64; do
    export OMP_NUM_THREADS=${threads}
    ./gol_omp -r -f ${GRID_FILE} -e ${MODE} -n ${STEPS}
done