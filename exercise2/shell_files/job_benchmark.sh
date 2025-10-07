#!/bin/bash
#SBATCH --job-name=gemm_benchmark
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=64
#SBATCH --partition=EPYC
#SBATCH --mem=128G
#SBATCH --time=02:00:00
#SBATCH --output=benchmark_%j.out
#SBATCH --error=benchmark_%j.err

./benchmark.sh
