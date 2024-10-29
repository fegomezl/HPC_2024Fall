#!/bin/bash

#SBATCH --job-name=rect8
#SBATCH --cluster=smp 
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=32
#SBATCH --cpus-per-task=1
#SBATCH --mail-user=feg46@pitt.edu
#SBATCH --mail-type=FAIL
#SBATCH --time=2:00:00
#SBATCH --qos=short
#SBATCH --output=results/rect_10_8.txt

for i in $(seq 1 32);
do
    for j in $(seq 1 10);
    do
        ./rect_par.x $i cluster
    done
done
