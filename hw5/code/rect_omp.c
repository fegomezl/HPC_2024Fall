#include <stdio.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <omp.h>

// System size
#define		N	256

// Structures for profilling
struct timeval startTime;
struct timeval finishTime;
double timeIntervalLength;

int main(int argc, char* argv[]){

	// Define variables
	int nproc;
	int i;
	double h;
	double area, local_area;
	
	// Initialize variables
	nproc = strtol(argv[1], NULL, 10);
	h = 10.0/N;
	area = 0.0;
	local_area = 0.0;

	// Get the start time
	gettimeofday(&startTime, NULL);

	// Divide sum among threads and reduce local result into global variable
    #pragma omp parallel num_threads(nproc) shared(h) private(i, local_area) reduction(+: area)
	{
		#pragma omp for
		for (i = 0; i < N; i++)
		    local_area += cos(i*h)*h;
		area += local_area;
	}

	// Get the end time
	gettimeofday(&finishTime, NULL);
		
	// Check results
	if (argc < 3)
		printf("Result : %.2lf \n", area);
	
	// Calculate the interval length 
	timeIntervalLength = (double)(finishTime.tv_sec-startTime.tv_sec)*1000000 
	                   + (double)(finishTime.tv_usec-startTime.tv_usec);
	timeIntervalLength = timeIntervalLength/1000;

	//Print the interval lenght
	if (argc < 3){
		printf("Interval length: %g msec.\n", timeIntervalLength);
	} else { 
		printf("%d\t%g\n", nproc, timeIntervalLength);
	}

	return 0;
}
