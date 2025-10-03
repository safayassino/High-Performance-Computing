#!/bin/bash
#SBATCH --job-name=gemm_thin
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=12
#SBATCH --partition=THIN
#SBATCH --mem=64G
#SBATCH --time=02:00:00
#SBATCH --output=benchmark_thin_%j.out
#SBATCH --error=benchmark_thin_%j.err

# Set node type environment variable
export NODE_TYPE=thin

./benchmark_thin.sh
