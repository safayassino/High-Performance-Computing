#!/bin/bash
#SBATCH --job-name=gemm_matrix_epyc
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=64
#SBATCH --partition=EPYC
#SBATCH --mem=64G
#SBATCH --time=01:00:00
#SBATCH --output=benchmark_matrix_epyc_%j.out
#SBATCH --error=benchmark_matrix_epyc_%j.err

export LD_LIBRARY_PATH=/u/dssc/syassin/myblis/lib:$LD_LIBRARY_PATH
./benchmark_matrix.sh
