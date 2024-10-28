#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <omp.h>

#define		NSTEPS	8388600
#define		NITER 	8388600
#define		P_START	0 
#define		P_END	10 


struct timeval startTime;
struct timeval finishTime;
double timeIntervalLength;

int main(int argc, char* argv[]){

	int i;
	double h;
	double area;

	//
	//I N I T I A L I Z A T I O N S
	//
	h = (double)(P_END-P_START)/NSTEPS;
	area = 0.0;

	//Get the start time
	gettimeofday(&startTime, NULL);

	// Read number of threads
    int nproc = strtol(argv[1], NULL, 10);

    // Divide sum among threads and reduce local result into global variable 
    double local_area;
    #pragma omp parallel num_threads(nproc) shared(h) private(i, local_area) reduction(+: area)
    {
        local_area = 0.0;
        #pragma omp for
        for (i = 0; i < NITER; i++)
            local_area += cos(P_START+i*h)*h;
        
        area += local_area;
    }

	//Get the end time
	gettimeofday(&finishTime, NULL);  /* after time */
		
	printf("Result : %.2lf \n", area);
	
	//Calculate the interval length 
	timeIntervalLength = (double)(finishTime.tv_sec-startTime.tv_sec) * 1000000 
		     + (double)(finishTime.tv_usec-startTime.tv_usec);
	timeIntervalLength = timeIntervalLength/1000;

	//Print the interval lenght
	printf("Interval length: %g msec.\n", timeIntervalLength);

	return 0;
}
