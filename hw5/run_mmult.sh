#!/bin/bash

#SBATCH --job-name=mmult
#SBATCH --cluster=gpu
#SBATCH --partition=gtx1080
#SBATCH --gres=gpu:1
#SBATCH --mail-user=feg46@pitt.edu
#SBATCH --mail-type=FAIL
#SBATCH --time=2:00:00
#SBATCH --qos=short
#SBATCH --output=results/mmult_4096.txt

module load cuda/11.0

make mmult_gpu

./mmult_gpu.x

#for i in $(seq 1 32);
#do
#    for j in $(seq 1 1);
#    do
#        ./mmult_gpu.x $i cluster
#    done
#done
