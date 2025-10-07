# Exercise 1

## Directory Structure
As explained in the report, I have opted to follow a pipeline in which I understood first how the GOL is implemented, serially, validate the correctness of the algorithm for both the static and ordered evolution by intializing for example the glider example and use the `*.pgm` th have an `*.png` that I can assess visually and then lastly apply parallization and study better the hpc constraints and implementation of this task on the cluster.
I implelemnted three scalability analysis: **OpenMP scalability** which I applied on both THIN and EPYC nodes, **Strong scalability** which I applied on THIN nodes on different matrix sizes, and lastly **Weak scalability**  using THIN node and across different matrix sizes (here I also added another visualization with 24 processes to challenge better the near-perfect weak scalability that I got at first). 

### CODE FILES
`game_of_life.h`: here  i make a header file containing data structures (the grid) and inline accessor functions for cell operations

`game_of_life_serial.c`:here i do the first serial implementation with both ORDERED and STATIC evolution modes (that will be the baseline for our analysis),  I take in consideration the periodic boundariesgit

`game_of_life_mpi.c`:here I implement the MPI domain (row) decomposition, ghost row exchanges via MPI_Sendrecv, and hybrid MPI+OpenMP evolution function.

`main.c`:here I added the command-line argument parsing, timing infrastructure, and execution control for baseline measurements

`main_omp.c`:to implement the OpenMP-parallelized evolution function with configurable thread counts

`main_mpi.c`:I used it to implement the MPI execution execution

`validate.c`:a validation tool where I generate PGM images of evolution steps, useful for visual correctness verification and debugging. (the glider example)

`Makefile`:the makefile helps us with the compilation of serial, OpenMP, MPI, and hybrid versions using the appropriate compiler flags


### SLURM Batch Scripts
`run_omp_scalability_thin.sh`:the sbatch file to run an  OpenMP scalability study on THIN node

`run_omp_scalability_epyc.sh`:the sbatch file to do OpenMP scalability on EPYC node

`run_strong_mpi.sh`:the sbatch file to run MPI strong scalability with 24 processes

`run_weak_scalability.sh`:the sbatch file to run Weak scalability study mainaining constant workload

`run_weak_scalability_24.sh`:the sbatch file to run weak scalability yet adding more procsses (24 processes)

`validate_correctness.sh`:the sbatch file I used to run the validate.c code with the images

## Output Files
`results_openmp.csv`:the data we get from the openMP scalability analysis on THIN node

`results_openmp_epyc.csv`:the data we get from the openMP scalability analysis on EPYC node

`results_strong_mpi.csv`:the data we get from the strong scalability analysis

`results_weak.csv`:the data we get from the weak scalability analysis

`*.png` :Visualizations of the speedup plots across all scalability analysis  + the examples of the grid states taken from the `*.pgm` files
