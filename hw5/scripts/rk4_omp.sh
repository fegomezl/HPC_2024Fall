#!/bin/bash

#SBATCH --job-name=rk4_omp
#SBATCH --cluster=smp
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=16
#SBATCH --cpus-per-task=1
#SBATCH --mail-user=feg46@pitt.edu
#SBATCH --mail-type=FAIL
#SBATCH --time=2:00:00
#SBATCH --qos=short
#SBATCH --output=results/rk4_omp-16384_%A.txt

make rk4_omp

for i in 4 8 16;
do
	for j in $(seq 1 10);
	do
		./rk4_omp.x $i cluster
	done
done
