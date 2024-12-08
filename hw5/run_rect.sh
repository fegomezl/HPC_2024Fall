#!/bin/bash

#SBATCH --job-name=rect
#SBATCH --cluster=gpu
#SBATCH --partition=gtx1080
#SBATCH --gres=gpu:1
#SBATCH --mail-user=feg46@pitt.edu
#SBATCH --mail-type=FAIL
#SBATCH --time=2:00:00
#SBATCH --qos=short
#SBATCH --output=results/rect_10_8.txt

module load cuda/11.0

make rect_gpu

./rect_gpu.x

#for i in $(seq 1 32);
#do
#    for j in $(seq 1 10);
#    do
#        ./rect_gpu.x $i cluster
#    done
#done