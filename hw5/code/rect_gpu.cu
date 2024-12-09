#include <stdio.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <cuda.h>

// System size
#define		N	1048576

// Structures for profilling
struct timeval startTime;
struct timeval finishTime;
double timeIntervalLength;

// Each thread sums its contribution to the global sum
__global__ void CUDA_rect(const double h, double* area){
	int my_tid = blockDim.x*blockIdx.x + threadIdx.x;
	if (my_tid < N)
		atomicAdd(area, cos(my_tid*h));
}

int main(int argc, char* argv[]){

	// Define variables
	double h;
	double *area;

	// Allocate variables
	cudaMallocManaged(&area, sizeof(double));
	
	// Initialize variables
	h = 10.0/N;
	*area = 0.0;

	// Get the start time
	gettimeofday(&startTime, NULL);

	// Run device kernel and syncronize device
	CUDA_rect<<<N/1024,1024>>>(h, area);
	cudaDeviceSynchronize();
	*area = h*(*area);

	// Get the end time
	gettimeofday(&finishTime, NULL);
		
	// Check results
	if (argc < 2)
		printf("Result : %.2lf \n", *area);
	
	// Calculate the interval length 
	timeIntervalLength = (double)(finishTime.tv_sec-startTime.tv_sec)*1000000 
	                   + (double)(finishTime.tv_usec-startTime.tv_usec);
	timeIntervalLength = timeIntervalLength/1000;

	//Print the interval lenght
	if (argc < 2){
		printf("Interval length: %g msec.\n", timeIntervalLength);
	} else { 
		printf("%g\n", timeIntervalLength);
	}

	// Free memory
	cudaFree(area);
        
	return 0;
}
