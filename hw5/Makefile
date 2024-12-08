mmult:
	gcc -g -Wall -O3 -o mmult.x code/mmult.c

rect:
	gcc -g -Wall -O3 -o rect.x code/rect.c -lm

rk4:
	gcc -g -Wall -O3 -o rk4.x code/rk4.c

mmult_omp:
	gcc -g -Wall -O3 -fopenmp -o mmult_omp.x code/mmult_omp.c

rect_omp:
	gcc -g -Wall -O3 -lm -fopenmp -o rect_omp.x code/rect_omp.c

rk4_omp:
	gcc -g -Wall -O3 -fopenmp -o rk4_omp.x code/rk4_omp.c

mmult_mpi:
	mpicc -g -Wall -O3 -o mmult_mpi.x code/mmult_mpi.c

rect_mpi:
	mpicc -g -Wall -O3 -lm -o rect_mpi.x code/rect_mpi.c

rk4_mpi:
	mpicc -g -Wall -O3 -o rk4_mpi.x code/rk4_mpi.c

mmult_gpu:
	nvcc -gencode arch=compute_61,code=[compute_61,sm_61] -O3 -o mmult_gpu.x code/mmult_gpu.cu

rect_gpu:
	nvcc -gencode arch=compute_61,code=[compute_61,sm_61] -O3 -o rect_gpu.x code/rect_gpu.cu

rk4_gpu:
	nvcc -gencode arch=compute_61,code=[compute_61,sm_61] -O3 -o rk4_gpu.x code/rk4_gpu.cu

see_smp:
	squeue -M smp -u feg46

see_gpu:
	squeue -M gpu -p gtx1080 -u feg46