# Exercise 2

## Directory Structure

### Shell Scripts
Contains four benchmark shell scripts for our different testing configurations:
- `benchmark.sh` - Fixed core + EPYC
- `benchmark_thin.sh` - Fixed core + THIN
- `benchmark_matrix.sh` - Fixed matrix + EPYC
- `benchmark_matrix_thin.sh` - Fixed matrix + THIN

### SBATCH Files
Four job sbatch files that call the past shell files while requesting adequate resources.

### Results Directories
- `fixed_cores_results/` - Data from the matrix scalability task (fixed cores, varying matrix size)
- `fixed_matrix_results/` - Data from the core scalability task (fixed matrix size, varying cores)

### Source Files
- `gemm.c` - Modified version of the provided `dgemm.c` code implementing matrix multiplication + added CSV output for results
- `gemm_numa.c` - NUMA-aware initialization implementation which I experimented with after observing the performance on the EPYC node