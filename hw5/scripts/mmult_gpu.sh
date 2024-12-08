#!/bin/bash

#SBATCH --job-name=mmult_gpu_%A
#SBATCH --cluster=gpu
#SBATCH --partition=gtx1080
#SBATCH --gres=gpu:1
#SBATCH --mail-user=feg46@pitt.edu
#SBATCH --mail-type=FAIL
#SBATCH --time=2:00:00
#SBATCH --qos=short
#SBATCH --output=results/mmult_gpu_256_%A.txt

module load cuda/11.0

make mmult_gpu

for i in $(seq 1 10);
do
    ./mmult_gpu.x cluster
done