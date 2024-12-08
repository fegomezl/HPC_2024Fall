#!/bin/bash

#SBATCH --job-name=rect
#SBATCH --cluster=smp
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=1
#SBATCH --mail-user=feg46@pitt.edu
#SBATCH --mail-type=FAIL
#SBATCH --time=2:00:00
#SBATCH --qos=short
#SBATCH --output=results/rect-256_%A.txt

make rect

for i in $(seq 1 10);
do
    ./rect.x cluster
done
