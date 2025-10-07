#!/bin/bash
#SBATCH --job-name=strong_hybrid_epyc
#SBATCH --output=strong_hybrid_epyc_%j.out
#SBATCH --error=strong_hybrid_epyc_%j.err
#SBATCH --nodes=2
#SBATCH --ntasks-per-node=2
#SBATCH --cpus-per-task=32
#SBATCH --time=01:00:00
#SBATCH --partition=EPYC

module load openMPI/4.1.6

export OMP_NUM_THREADS=32

cd $SLURM_SUBMIT_DIR

ROWS=20000
COLS=20000
STEPS=50
MODE=1
CSV_FILE="results_strong_hybrid_epyc_final.csv"
GRID_FILE="grid_${ROWS}.pgm"

if [ ! -f ${CSV_FILE} ]; then
    echo "study,rows,cols,steps,mode,mpi_procs,omp_threads,time" > ${CSV_FILE}
fi

if [ ! -f ${GRID_FILE} ]; then
    ./gol_serial -i -k ${ROWS},${COLS} -f ${GRID_FILE}
fi

rm -f results_mpi.csv
echo "study,rows,cols,steps,mode,mpi_procs,omp_threads,time" > results_mpi.csv

for nprocs in 1 2 4; do
    mpirun -np ${nprocs} ./gol_mpi -r -f ${GRID_FILE} -e ${MODE} -n ${STEPS}
done

tail -n +2 results_mpi.csv >> ${CSV_FILE}