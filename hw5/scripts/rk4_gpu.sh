#!/bin/bash

#SBATCH --job-name=rk4_gpu
#SBATCH --cluster=gpu
#SBATCH --partition=gtx1080
#SBATCH --gres=gpu:1
#SBATCH --mail-user=feg46@pitt.edu
#SBATCH --mail-type=FAIL
#SBATCH --time=2:00:00
#SBATCH --qos=short
#SBATCH --output=results/rk4_gpu-4096_%A.txt

module load cuda/11.0

make rk4_gpu

for i in $(seq 1 30);
do
    ./rk4_gpu.x cluster
done
