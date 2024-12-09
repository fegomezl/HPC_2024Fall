#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h> 

// System size
#define		N	4096

// Find first index of pid for a load-balanced partition of N items
int partition_index(long size, long pid, long nproc);

int main(int argc, char* argv[]){
	
    	// Define variables
    	int pid, nproc;
	double h, totalSum;
	int first_index, last_index, i, j;
    	double local_start, local_end, local_elapsed, elapsed;
	double*  y;
	double*  k1;
	double*  k2;
	double*  k3;
	double*  k4;
	double*  local_k1;
	double*  local_k2;
	double*  local_k3;
	double*  local_k4;
	double*  pow;
	double*  yout;
    	double*  local_yout;
	double* c;
	
	// Allocate variables
	y    = (double*)malloc(N*sizeof(double));
	k1   = (double*)malloc(N*sizeof(double));
	k2   = (double*)malloc(N*sizeof(double));
	k3   = (double*)malloc(N*sizeof(double));
	k4   = (double*)malloc(N*sizeof(double));
	local_k1   = (double*)malloc(N*sizeof(double));
	local_k2   = (double*)malloc(N*sizeof(double));
	local_k3   = (double*)malloc(N*sizeof(double));
	local_k4   = (double*)malloc(N*sizeof(double));
	pow  = (double*)malloc(N*sizeof(double));
	yout = (double*)malloc(N*sizeof(double));
	local_yout = (double*)malloc(N*sizeof(double));
	c    = (double*)malloc(N*N*sizeof(double));

    	// Start MPI code
    	MPI_Init(&argc, &argv);
    	MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    	MPI_Comm_size(MPI_COMM_WORLD, &nproc);
	
	// Initialize variables
	h = 0.3154;
	totalSum = 0.0;
    	first_index = partition_index(N, pid, nproc);
    	last_index = partition_index(N, pid+1, nproc);
	for (i = 0; i < N; i++){
		y[i] = i*i;
		pow[i] = 2*i;
		for (j = 0; j < N; j++)
			c[i*N+j] = i*i+j;
		local_k1[i] = 0;
		local_k2[i] = 0;
		local_k3[i] = 0;
		local_k4[i] = 0;
		local_yout[i] = 0;
	}

    	// Get the start time
    	MPI_Barrier(MPI_COMM_WORLD);
    	local_start = MPI_Wtime();

	// Apply RK4 method, calculating each step one at a time
	for (i = first_index; i < last_index; i++){ 
		local_k1[i] = pow[i];
		for (j = 0; j < N; j++)
			local_k1[i] -= c[i*N+j]*y[j];
		local_k1[i] *= h;
	}
    	MPI_Allreduce(local_k1, k1, N, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

	for (i = first_index; i < last_index; i++){ 
		local_k2[i] = pow[i];
		for (j = 0; j < N; j++)
			local_k2[i] -= c[i*N+j]*(y[j]+0.5*k1[j]);
		local_k2[i] *= h;
	}
    	MPI_Allreduce(local_k2, k2, N, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

	for (i = first_index; i < last_index; i++){ 
		local_k3[i] = pow[i];
		for (j = 0; j < N; j++)
			local_k3[i] -= c[i*N+j]*(y[j]+0.5*k2[j]);
		local_k3[i] *= h;
	}
    	MPI_Allreduce(local_k3, k3, N, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

	for (i = first_index; i < last_index; i++){ 
		local_k4[i] = pow[i];
		for (j = 0; j < N; j++)
			local_k4[i] -= c[i*N+j]*(y[j]+k3[j]);
		local_k4[i] *= h;

        local_yout[i] = y[i] + (local_k1[i]+2*local_k2[i]+2*local_k3[i]+local_k4[i])/6.0;
	}
    	MPI_Allreduce(local_k4, k4, N, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
   	MPI_Reduce(local_yout, yout, N, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	// Get the end time
	local_end = MPI_Wtime();
	local_elapsed = (local_end - local_start)*1000; // Time in ms
	MPI_Reduce(&local_elapsed, &elapsed, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

	// Check results
	if (pid == 0 && argc == 1){
		totalSum = 0;
		for (i = 0; i < N; i++)
			totalSum += yout[i];
		printf("Total Sum = %g\n", totalSum);
	}

	// Print the interval lenght
	if (pid == 0){
		if (argc == 1)
	    		printf("Interval length: %g msec.\n", elapsed);
		else
	    		printf("%d\t%g\n", nproc, elapsed);
	}

	// Finish program
	MPI_Finalize();

	return 0;
}

// Find first index of pid for a load-balanced partition of N items
int partition_index(long size, long pid, long nproc){
	int ratio = size/nproc;
	int reminder = size%nproc;

	if (pid < reminder){
		return (ratio+1)*pid;
	} else if (pid < nproc) { 
		return ratio*pid + reminder;
	} else {
		return size;
	}
}
