#include <stdio.h>
#include <time.h>
#include <sys/time.h> 
#include <cuda.h>

#define		N	1024

struct timeval startTime;
struct timeval finishTime;
double timeIntervalLength;

__global__ void CUDA_mmult(const double inputArrayA[], const double inputArrayB[], double outputArrayC[]){
	int my_tid = blockDim.x*blockIdx.x + threadIdx.x;
	int i = my_tid/N;
	int j = my_tid%N;
	if (0 < my_tid && my_tid < N*N){
		int k;
		double my_C = 0.0;
		for (k = 0; k < N; k++)
			my_C += inputArrayA[i*N+k]*inputArrayB[k*N+j];
		outputArrayC[i*N+j] = my_C;
	}
}

int main(int argc, char* argv[]){

	// Define variables
	int i, j;
	double totalSum;
	double expectedSum;
	double* inputArrayA;
	double* inputArrayB;
	double* outputArrayC;

	// Allocate arrays
	cudaMallocManaged(&inputArrayA, N*N*sizeof(double));
	cudaMallocManaged(&inputArrayB, N*N*sizeof(double));
	cudaMallocManaged(&outputArrayC, N*N*sizeof(double));
	
	// Initialize arrays
	for (i = 0; i < N; i++){
		for (j = 0; j < N; j++){
			inputArrayA[i*N+j] = i*N+j;
			inputArrayB[i*N+j] = j*(N+1);
			outputArrayC[i*N+j] = 0.0;
		}
	}

	// Get the start time
	gettimeofday(&startTime, NULL);
   
	// Matrix multiplication
	CUDA_mmult<<<1024,1024>>>(inputArrayA, inputArrayB, outputArrayC);
	cudaDeviceSynchronize();
	
	// Get the end time
	gettimeofday(&finishTime, NULL);

	// Check results
	totalSum = 0;
	expectedSum = 0;
	for (i = 0; i < N; i++){
		for (j = 0; j < N; j++){
			totalSum += outputArrayC[i*N+j];
			expectedSum += 1.0*(N*N*i + (N*(N-1))/2)*j*(N+1);
		}
	}
	printf("\nTotal Sum = %g\n", totalSum);
	printf("\nExpected Sum = %g\n", expectedSum);

	//Calculate the interval length 
	timeIntervalLength = (double)(finishTime.tv_sec-startTime.tv_sec) * 1000000 
	                   + (double)(finishTime.tv_usec-startTime.tv_usec);
	timeIntervalLength = timeIntervalLength/1000;

	//Print the interval length
	printf("Interval length: %g msec.\n", timeIntervalLength);

	// Free memory
	cudaFree(inputArrayA);
	cudaFree(inputArrayB);
	cudaFree(outputArrayC);

	return 0;  
}
