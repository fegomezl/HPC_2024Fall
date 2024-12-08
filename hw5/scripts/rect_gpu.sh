#!/bin/bash

#SBATCH --job-name=rect_gpu_%A
#SBATCH --cluster=gpu
#SBATCH --partition=gtx1080
#SBATCH --gres=gpu:1
#SBATCH --mail-user=feg46@pitt.edu
#SBATCH --mail-type=FAIL
#SBATCH --time=2:00:00
#SBATCH --qos=short
#SBATCH --output=results/rect_gpu_256_%A.txt

module load cuda/11.0

make rect_gpu

for i in $(seq 1 10);
do
    ./rect_gpu.x cluster
done