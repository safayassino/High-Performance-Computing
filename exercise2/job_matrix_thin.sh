#!/bin/bash
#SBATCH --job-name=gemm_matrix_thin
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=12
#SBATCH --partition=THIN
#SBATCH --mem=64G
#SBATCH --time=01:00:00
#SBATCH --output=benchmark_matrix_thin_%j.out
#SBATCH --error=benchmark_matrix_thin_%j.err

./benchmark_matrix_thin.sh
