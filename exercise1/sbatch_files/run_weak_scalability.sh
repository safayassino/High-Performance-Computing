#!/bin/bash
#SBATCH --job-name=gol_weak
#SBATCH --output=weak_%j.out
#SBATCH --error=weak_%j.err
#SBATCH --nodes=2
#SBATCH --ntasks-per-node=2
#SBATCH --cpus-per-task=6
#SBATCH --time=02:00:00
#SBATCH --partition=THIN

module load openMPI/4.1.6

cd $SLURM_SUBMIT_DIR

BASE_ROWS=10000
BASE_COLS=10000
STEPS=50
MODE=1
CSV_FILE="results_weak.csv"

export OMP_NUM_THREADS=6

if [ ! -f ${CSV_FILE} ]; then
    echo "study,rows,cols,steps,mode,mpi_procs,omp_threads,time" > ${CSV_FILE}
fi

rm -f results_mpi.csv
echo "study,rows,cols,steps,mode,mpi_procs,omp_threads,time" > results_mpi.csv

for nprocs in 1 2 4; do
    ROWS=${BASE_ROWS}
    COLS=$((BASE_COLS * nprocs))
    GRID_FILE="grid_${BASE_ROWS}_${nprocs}.pgm"
    
    if [ ! -f ${GRID_FILE} ]; then
        ./gol_serial -i -k ${ROWS},${COLS} -f ${GRID_FILE}
    fi
    
    mpirun -np ${nprocs} ./gol_mpi -r -f ${GRID_FILE} -e ${MODE} -n ${STEPS}
done

tail -n +2 results_mpi.csv >> ${CSV_FILE}
