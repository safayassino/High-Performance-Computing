# Exercise 2

## Directory Structure

### Shell Scripts
Contains four benchmark shell scripts for different testing configurations:
- `benchmark.sh` - Fixed core count benchmarks on EPYC nodes
- `benchmark_thin.sh` - Fixed core count benchmarks on thin nodes
- `benchmark_matrix.sh` - Fixed matrix size benchmarks on EPYC nodes
- `benchmark_matrix_thin.sh` - Fixed matrix size benchmarks on thin nodes

### SBATCH Files
Four SLURM job submission scripts that execute the corresponding shell scripts with appropriate resource allocations.

### Results Directories
- `fixed_cores_results/` - Data from matrix scalability experiments (fixed core count, varying matrix size)
- `fixed_matrix_results/` - Data from core scalability experiments (fixed matrix size, varying core count)

### Source Files
- `gemm.c` - Modified version of the provided `dgemm.c` code implementing matrix multiplication with optional CSV output for results
- `gemm_numa.c` - NUMA-aware initialization implementation added after observing performance characteristics on EPYC nodes