#!/bin/bash

#SBATCH --job-name=mmult_gpu
#SBATCH --cluster=gpu
#SBATCH --partition=gtx1080
#SBATCH --gres=gpu:1
#SBATCH --mail-user=feg46@pitt.edu
#SBATCH --mail-type=FAIL
#SBATCH --time=2:00:00
#SBATCH --qos=short
#SBATCH --output=results/mmult_gpu-1024_%A.txt

module load cuda/11.0

make mmult_gpu

for i in $(seq 1 30);
do
    ./mmult_gpu.x cluster
done
