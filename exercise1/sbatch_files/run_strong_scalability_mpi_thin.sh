#!/bin/bash
#SBATCH --job-name=strong_pure_mpi
#SBATCH --output=strong_pure_mpi_%j.out
#SBATCH --error=strong_pure_mpi_%j.err
#SBATCH --nodes=2
#SBATCH --ntasks=24
#SBATCH --cpus-per-task=1
#SBATCH --time=02:00:00
#SBATCH --partition=THIN

module load openMPI/4.1.6

export OMP_NUM_THREADS=1

cd $SLURM_SUBMIT_DIR

ROWS=20000
COLS=20000
STEPS=50
MODE=1
CSV_FILE="results_strong_pure_mpi.csv"
GRID_FILE="grid_${ROWS}.pgm"

if [ ! -f ${CSV_FILE} ]; then
    echo "study,rows,cols,steps,mode,mpi_procs,omp_threads,time" > ${CSV_FILE}
fi

if [ ! -f ${GRID_FILE} ]; then
    ./gol_serial -i -k ${ROWS},${COLS} -f ${GRID_FILE}
fi

rm -f results_mpi.csv
echo "study,rows,cols,steps,mode,mpi_procs,omp_threads,time" > results_mpi.csv

for nprocs in 1 2 4 6 8 12 16 20 24; do
    mpirun -np ${nprocs} ./gol_mpi -r -f ${GRID_FILE} -e ${MODE} -n ${STEPS}
done

tail -n +2 results_mpi.csv >> ${CSV_FILE}