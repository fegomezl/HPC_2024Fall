#!/bin/bash

#SBATCH --job-name=rk4_8192
#SBATCH --cluster=smp
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=32
#SBATCH --cpus-per-task=1
#SBATCH --mail-user=feg46@pitt.edu
#SBATCH --mail-type=FAIL
#SBATCH --time=2:00:00
#SBATCH --qos=short
#SBATCH --output=results/rk4_8192_%A.txt

module load openmpi/4.0.5

make rk4_mpi

for i in $(seq 1 32);
do
	mpirun -n $i ./rk4_mpi.x cluster
done
