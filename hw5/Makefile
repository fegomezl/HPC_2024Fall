mmult:
	gcc -g -Wall -O3 -o mmult.x code/mmult.c

rect:
	gcc -g -Wall -O3 -o rect.x code/rect.c -lm

rk4:
	gcc -g -Wall -O3 -o rk4.x code/rk4.c

mmult_gpu:
	nvcc -gencode arch=compute_70,code=[compute_70,sm_70] -o mmult_gpu.x mmult_gpu.cu
#  -g -Wall -O3

rect_gpu:
	nvcc -gencode arch=compute_70,code=[compute_70,sm_70] -o rect_gpu.x rect_gpu.cu

rk4_gpu:
	nvcc -gencode arch=compute_70,code=[compute_70,sm_70] -o rk4_gpu.x rk4_gpu.cu
