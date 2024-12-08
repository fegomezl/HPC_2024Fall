#!/bin/bash

#SBATCH --job-name=rk4_%A
#SBATCH --cluster=smp
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=1
#SBATCH --mail-user=feg46@pitt.edu
#SBATCH --mail-type=FAIL
#SBATCH --time=2:00:00
#SBATCH --qos=short
#SBATCH --output=results/rk4_256_%A.txt

make rk4

for i in $(seq 1 10);
do
    ./rk4.x cluster
done