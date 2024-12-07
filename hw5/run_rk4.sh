#!/bin/bash

#SBATCH --job-name=rk_15000
#SBATCH --cluster=smp
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=32
#SBATCH --cpus-per-task=1
#SBATCH --mail-user=feg46@pitt.edu
#SBATCH --mail-type=FAIL
#SBATCH --time=2:00:00
#SBATCH --qos=short
#SBATCH --output=results/rk_15000.txt

for i in $(seq 1 32);
do
    for j in $(seq 1 30);
    do
        ./rk3_8_par.x $i cluster
    done
done
