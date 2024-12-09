#include <stdio.h>
#include <stdlib.h>
#include <mpi.h> 

// System size
#define		N	1024

// Find first index of pid for a load-balanced partition of N items
int partition_index(long size, long pid, long nproc);

int main(int argc, char* argv[]){
	
	// Define variables
	int pid, nproc;
	int first_index, last_index, n, i, j, k;
	double totalSum, expectedSum;
	double local_start, local_end, local_elapsed, elapsed;
	double* inputArrayA;
	double* inputArrayB;
	double* localArrayC;
	double* outputArrayC;

	// Start MPI code
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &pid);
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);

	// Allocate variables
	inputArrayA = (double*)malloc(N*N*sizeof(double));
	inputArrayB = (double*)malloc(N*N*sizeof(double));
	localArrayC = (double*)malloc(N*N*sizeof(double));
	outputArrayC = (double*)malloc(N*N*sizeof(double));

	// Initialize variables
	first_index = partition_index(N*N, pid, nproc);
	last_index = partition_index(N*N, pid+1, nproc);
	for (i = 0; i < N; i++){
		for (j = 0; j < N; j++){
			inputArrayA[i*N+j] = i*N+j;
			inputArrayB[i*N+j] = j*(N+1);
			localArrayC[i*N+j] = 0.0;
			outputArrayC[i*N+j] = 0.0;
		}
	}

	// Get the start time
	MPI_Barrier(MPI_COMM_WORLD);
	local_start = MPI_Wtime();

	// Divide rows and columns of resulting matrix among threads
	for (n = first_index; n < last_index; n++){
		i = n/N;
		j = n%N;
		for (k = 0; k < N; k++)
			localArrayC[n] += inputArrayA[i*N+k]*inputArrayB[k*N+j];
	}
	// Accumulate sum in master processor
	MPI_Reduce(localArrayC, outputArrayC, N*N, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	// Get the end time
	local_end = MPI_Wtime();
	local_elapsed = (local_end - local_start)*1000; // Time in ms
	MPI_Reduce(&local_elapsed, &elapsed, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

	// Check results
	if (pid == 0 && argc == 1){
		totalSum = 0;
		expectedSum = 0;
		for (i = 0; i < N; i++){
			for (j = 0; j < N; j++){
				totalSum += outputArrayC[i*N+j];
				expectedSum += 1.0*(N*N*i + (N*(N-1))/2)*j*(N+1);
			}
		}
		printf("\nTotal Sum = %g\n", totalSum);
		printf("Expected Sum = %g\n", expectedSum);
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
