#!/bin/bash

#SBATCH --job-name=mmult_4096
#SBATCH --cluster=smp
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=32
#SBATCH --cpus-per-task=1
#SBATCH --mail-user=feg46@pitt.edu
#SBATCH --mail-type=FAIL
#SBATCH --time=2:00:00
#SBATCH --qos=short
#SBATCH --output=results/mmult_4096.txt

for i in $(seq 1 32);
do
    for j in $(seq 1 1);
    do
        ./mmult_mpi.x $i cluster
    done
done
