#include <stdio.h>
#include <math.h>
#include <mpi.h>

#define		NSTEPS	8388608
#define		NITER 	8388608
#define		P_START	0 
#define		P_END	10 

// Find first index of pid for a load-balanced partition of N items
int partition_index(long N, long pid, long nproc);

int main(int argc, char* argv[]){

    // Define variables
    int pid, nproc;
    double local_start, local_end, local_elapsed, elapsed;
	double h;
	double local_area, area;
	int first_index, last_index, i;

    // Start MPI code
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);

    // Initialize variables
    h = (double)(P_END-P_START)/NSTEPS;
    local_area = 0.0;
    area = 0.0;
    first_index = partition_index(NITER, pid, nproc);
    last_index = partition_index(NITER, pid+1, nproc);

    // Get the start time
    MPI_Barrier(MPI_COMM_WORLD);
    local_start = MPI_Wtime();

    // Calculate local area
    for (i = first_index; i < last_index; i++)
        local_area += 0.5*h*(sin(P_START+i*h)+sin(P_START+i*h));
    // Accumulate sum in master processor
    MPI_Reduce(&local_area, &area, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    // Get the end time
    local_end = MPI_Wtime();
    local_elapsed = (local_end - local_start)*1000; // Time in ms
    MPI_Reduce(&local_elapsed, &elapsed, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
 
    // Check results
    if (pid == 0 && argc == 1)
        printf("Result : %.2lf \n", area);

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
