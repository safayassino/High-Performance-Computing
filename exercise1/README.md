# Exercise 1

## Directory Structure

### `game_of_life.h`: 
Header file containing data structures (Grid), function prototypes, and inline accessor functions for cell operations.

### `game_of_life_serial.c`
here i do the first serial implementation with both ORDERED and STATIC evolution modes (that will be the baseline for our analysis),  I take in consideration the periodic boundaries, PGM I/O (which I use to create png images, the glider example is mentioned in the report)

### `game_of_life_mpi.c`
here I implement the MPI domain (row) decomposition, ghost row exchanges via MPI_Sendrecv, and hybrid MPI+OpenMP evolution function.

### `main.c`
here I added the command-line argument parsing, timing infrastructure, and execution control for baseline measurements.

### `main_omp.c`
to implement the OpenMP-parallelized evolution function with configurable thread counts.

### `main_mpi.c`
I used it to implement the MPI execution execution.

### `validate.c`
a validation tool where I generate PGM images of evolution steps, useful for visual correctness verification and debugging. (the glider example)

### `Makefile`
the makefile helps us with the compilation of serial, OpenMP, MPI, and hybrid versions using the appropriate compiler flags.

## SLURM Batch Scripts

### `run_omp_scalability_thin.sh`
the sbatch file to run an  OpenMP scalability study on THIN node

### `run_omp_scalability_epyc.sh`
the sbatch file to do OpenMP scalability on EPYC node

### `run_strong_mpi.sh`
the sbatch file to run MPI strong scalability with 24 processes.

### `run_weak_scalability.sh`
the sbatch file to run Weak scalability study mainaining constant workload.

### `run_weak_scalability_24.sh` 
the sbatch file to run weak scalability yet adding more procsses (24 processes)

### `validate_correctness.sh`
the sbatch file I used to run the validate.c code with the images


## Output Files

### `results_openmp.csv`
the data we get from the openMP scalability analysis

### `results_strong_mpi.csv`
the data we get from the strong scalability analysis

### `results_weak.csv`
the data we get from the weak scalability analysis

### `*.pgm`
PGM image files storing grid states for initialization and validation purposes

### `*.png` 
Visualizations of the speedup changes across all scalability analysis