#include <stdio.h>
#include <math.h>
#include <mpi.h> 

#define		NROW	256
#define		NCOL	NROW

// Find first index of pid for a load-balanced partition of N items
int partition_index(long N, long pid, long nproc);

int main(int argc, char* argv[]){

    // Define variables
    int pid, nproc;
    double local_start, local_end, local_elapsed, elapsed;
    int inputArrayA  [NROW*NCOL];
    int inputArrayB  [NROW*NCOL];
    int localArrayC  [NROW*NCOL];
    int outputArrayC [NROW*NCOL];
	int first_index, last_index, n, i, j, k;
	double totalSum;

    // Start MPI code
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);
	
	// Initialize arrays and variables
	for(i = 0; i < NROW; i++){
		for(j = 0; j < NCOL; j++){
			inputArrayA[i*NCOL+j] = i*NCOL + j;
			inputArrayB[i*NCOL+j] = j*NCOL + i;
			localArrayC[i*NCOL+j] = 0;
			outputArrayC[i*NCOL+j] = 0;
		}
	}
    first_index = partition_index(NCOL*NROW, pid, nproc);
    last_index = partition_index(NCOL*NROW, pid+1, nproc);

    // Get the start time
    MPI_Barrier(MPI_COMM_WORLD);
    local_start = MPI_Wtime();

    // Divide rows and columns of resulting matrix among threads
    for (n = first_index; n < last_index; n++){
        i = n/NCOL;
        j = n%NCOL;
        for (k = 0; k < NROW; k++)
            localArrayC[i*NCOL+j] += inputArrayA[i*NCOL+k]*inputArrayB[k*NCOL+j];
    }
    // Accumulate sum in master processor
    MPI_Reduce(localArrayC, outputArrayC, NROW*NCOL, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	
    // Get the end time
    local_end = MPI_Wtime();
    local_elapsed = (local_end - local_start)*1000; // Time in ms
    MPI_Reduce(&local_elapsed, &elapsed, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    // Check results
    if (pid == 0 && argc == 1){
        totalSum = 0;
        for (i = 0; i < NROW; i++){
			for (j = 0; j < NCOL; j++){
				totalSum += (double)outputArrayC[i*NCOL+j];
			}
		}
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
