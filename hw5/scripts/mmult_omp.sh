#!/bin/bash

#SBATCH --job-name=mmult_omp
#SBATCH --cluster=smp
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=32
#SBATCH --cpus-per-task=1
#SBATCH --mail-user=feg46@pitt.edu
#SBATCH --mail-type=FAIL
#SBATCH --time=2:00:00
#SBATCH --qos=short
#SBATCH --output=results/mmult_omp-256_%A.txt

make mmult_omp

for i in $(seq 1 32);
do
    ./mmult_omp.x $i cluster
done
