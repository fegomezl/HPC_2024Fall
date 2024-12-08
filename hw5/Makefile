mmult:
	gcc -g -Wall -O3 -o mmult.x code/mmult.c

rect:
	gcc -g -Wall -O3 -o rect.x code/rect.c -lm

rk4:
	gcc -g -Wall -O3 -o rk4.x code/rk4.c

mmult_gpu:
	nvcc -gencode arch=compute_61,code=[compute_61,sm_61] -o mmult_gpu.x code/mmult_gpu.cu
#  -g -Wall -O3

rect_gpu:
	nvcc -gencode arch=compute_61,code=[compute_61,sm_61] -o rect_gpu.x code/rect_gpu.cu

rk4_gpu:
	nvcc -gencode arch=compute_61,code=[compute_61,sm_61] -o rk4_gpu.x code/rk4_gpu.cu

check_gpu:
	squeue -M gpu -p gtx1080 -u feg46
