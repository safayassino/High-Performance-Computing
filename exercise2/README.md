# information about exercise 2
##shell files: contains 4 benchmark shell scripts: benchmark.sh (fixed cores, epyc), benchmark_thin.sh (fixed cores, thin), benchmark_matrix.sh (fixed matrix, epyc), benchmark_matrix_thin.sh (fixed matrix, thin)
##sbatch files: four job sbatch files that call the past shell files while requesting adequate resources
##fixed_cores_results: contains the data for the matrix scalability (fixed core)
##fixed_matrix_results: contains the data for the core scalability (fixed matrix)