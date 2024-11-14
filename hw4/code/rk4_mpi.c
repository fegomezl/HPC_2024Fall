#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h> 

#define PROBLEM_SIZE 8192
#define h 0.3154

// Find first index of pid for a load-balanced partition of N items
int partition_index(long N, long pid, long nproc);

int main(int argc, char* argv[]){

    // Define variables
    int pid, nproc;
    double local_start, local_end, local_elapsed, elapsed;
	int first_index, last_index, i, j;
	double*  y;
	double*  k1;
	double*  k2;
	double*  k3;
	double*  local_k1;
	double*  local_k2;
	double*  local_k3;
	double*  local_k4;
	double*  pow;
	double*  yout;
    double*  local_yout;
	double** c;
	double totalSum;

    // Start MPI code
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);

	// Initialize arrays and variables
	y    = (double* )malloc(PROBLEM_SIZE*sizeof(double));
	k1   = (double* )malloc(PROBLEM_SIZE*sizeof(double));
	k2   = (double* )malloc(PROBLEM_SIZE*sizeof(double));
	k3   = (double* )malloc(PROBLEM_SIZE*sizeof(double));
	local_k1   = (double* )malloc(PROBLEM_SIZE*sizeof(double));
	local_k2   = (double* )malloc(PROBLEM_SIZE*sizeof(double));
	local_k3   = (double* )malloc(PROBLEM_SIZE*sizeof(double));
	local_k4   = (double* )malloc(PROBLEM_SIZE*sizeof(double));
	pow  = (double* )malloc(PROBLEM_SIZE*sizeof(double));
	yout = (double* )malloc(PROBLEM_SIZE*sizeof(double));
	local_yout = (double* )malloc(PROBLEM_SIZE*sizeof(double));
	c    = (double**)malloc(PROBLEM_SIZE*sizeof(double*));
	for (i = 0; i < PROBLEM_SIZE; i++)
		c[i] = (double*)malloc(PROBLEM_SIZE*sizeof(double));

	for (i = 0; i < PROBLEM_SIZE; i++){
		y[i] = i*i;
		pow[i] = 2*i;
		for (j = 0; j < PROBLEM_SIZE; j++){
			c[i][j] = i*i+j;
		}
		local_k1[i] = 0;
		local_k2[i] = 0;
		local_k3[i] = 0;
		local_k4[i] = 0;
		local_yout[i] = 0;
	}
    first_index = partition_index(PROBLEM_SIZE, pid, nproc);
    last_index = partition_index(PROBLEM_SIZE, pid+1, nproc);

    // Get the start time
    MPI_Barrier(MPI_COMM_WORLD);
    local_start = MPI_Wtime();

	for (i = first_index; i < last_index; i++){ 
		local_k1[i] = h*pow[i];
		for (j = 0; j < PROBLEM_SIZE; j++)
			local_k1[i] -= h*c[i][j]*y[j];
	}
    MPI_Allreduce(local_k1, k1, PROBLEM_SIZE, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

	for (i = first_index; i < last_index; i++){ 
		local_k2[i] = h*pow[i];
		for (j = 0; j < PROBLEM_SIZE; j++)
			local_k2[i] -= h*c[i][j]*(y[j]+0.5*k1[j]);
	}
    MPI_Allreduce(local_k2, k2, PROBLEM_SIZE, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

	for (i = first_index; i < last_index; i++){ 
		local_k3[i] = h*pow[i];
		for (j = 0; j < PROBLEM_SIZE; j++)
			local_k3[i] -= h*c[i][j]*(y[j]+0.5*k2[j]);
	}
    MPI_Allreduce(local_k3, k3, PROBLEM_SIZE, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

	for (i = first_index; i < last_index; i++){ 
		local_k4[i] = h*pow[i];
		for (j = 0; j < PROBLEM_SIZE; j++)
			local_k4[i] -= h*c[i][j]*(y[j]+k3[j]);

        local_yout[i] = y[i] + (local_k1[i]+2*local_k2[i]+2*local_k3[i]+local_k4[i])/6.0;
	}
    MPI_Reduce(local_yout, yout, PROBLEM_SIZE, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    // Get the end time
    local_end = MPI_Wtime();
    local_elapsed = (local_end - local_start)*1000; // Time in ms
    MPI_Reduce(&local_elapsed, &elapsed, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    // Check results
    if (pid == 0 && argc == 1){
        totalSum = 0;
        for (i = 0; i < PROBLEM_SIZE; i++)
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
int partition_index(long N, long pid, long nproc){
	int ratio = N/nproc;
	int reminder = N%nproc;

	if (pid < reminder){
		return (ratio+1)*pid;
	} else if (pid < nproc) { 
		return ratio*pid + reminder;
	} else {
		return N;
	}
}
