#include <stdio.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <cuda.h>

#define		NSTEPS	1048576
#define		P_START	0 
#define		P_END	10 

struct timeval startTime;
struct timeval finishTime;
double timeIntervalLength;

__global__ void CUDA_rect(const double h, double* area){
	int my_tid = blockDim.x*blockIdx.x + threadIdx.x;
	if (0 < my_tid && my_tid < NSTEPS)
		atomicAdd(area, cos(P_START+my_tid*h));
}

int main(int argc, char* argv[]){
	// Define variables
	double h;
	double *area;

	// Allocate variables
	cudaMallocManaged(&area, sizeof(double));
	
	// Initialize variables
	h = (double)(P_END-P_START)/NSTEPS;
	*area = 0.0;

	// Get the start time
	gettimeofday(&startTime, NULL);

	CUDA_rect<<<1024,1024>>>(h, area);
	cudaDeviceSynchronize();
	*area = h*(*area);

	// Get the end time
	gettimeofday(&finishTime, NULL);
	
	// Check results
	printf("Result : %.2lf \n", *area);
	
	// Calculate the interval length 
	timeIntervalLength = (double)(finishTime.tv_sec-startTime.tv_sec) * 1000000 
		     		   + (double)(finishTime.tv_usec-startTime.tv_usec);
	timeIntervalLength = timeIntervalLength/1000;

	// Print the interval lenght
	printf("Interval length: %g msec.\n", timeIntervalLength);

	// Free memory
	cudaFree(area);
        
	return 0;
}
