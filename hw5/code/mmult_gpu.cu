#include <stdio.h>
#include <time.h>
#include <sys/time.h> 
#include <cuda.h>

// System size
#define		N	1024

// Structures for profilling
struct timeval startTime;
struct timeval finishTime;
double timeIntervalLength;

// Perform dot product for matrix multiplication on each thread
__global__ void CUDA_mmult(const double inputArrayA[], const double inputArrayB[], double outputArrayC[]){
	int my_tid = blockDim.x*blockIdx.x + threadIdx.x;
	int i = my_tid/N;
	int j = my_tid%N;
	if (my_tid < N*N){
		int k;
		double my_C = 0.0;
		for (k = 0; k < N; k++)
			my_C += inputArrayA[i*N+k]*inputArrayB[k*N+j];
		outputArrayC[my_tid] = my_C;
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

	// Allocate variables
	cudaMallocManaged(&inputArrayA, N*N*sizeof(double));
	cudaMallocManaged(&inputArrayB, N*N*sizeof(double));
	cudaMallocManaged(&outputArrayC, N*N*sizeof(double));
	
	// Initialize variables
	for (i = 0; i < N; i++){
		for (j = 0; j < N; j++){
			inputArrayA[i*N+j] = i*N+j;
			inputArrayB[i*N+j] = j*(N+1);
			outputArrayC[i*N+j] = 0.0;
		}
	}

	// Get the start time
	gettimeofday(&startTime, NULL);
   
	// Call device function and synchronize
	CUDA_mmult<<<N*N/1024,1024>>>(inputArrayA, inputArrayB, outputArrayC);
	cudaDeviceSynchronize();
	
	// Get the end time
	gettimeofday(&finishTime, NULL);

	// Check results
	if (argc < 2){
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

	//Calculate the interval length 
	timeIntervalLength = (double)(finishTime.tv_sec-startTime.tv_sec)*1000000 
	                   + (double)(finishTime.tv_usec-startTime.tv_usec);
	timeIntervalLength = timeIntervalLength/1000;

	//Print the interval length
	if (argc < 2){
		printf("Interval length: %g msec.\n", timeIntervalLength);
	} else { 
		printf("%g\n", timeIntervalLength);
	}

	// Free memory
	cudaFree(inputArrayA);
	cudaFree(inputArrayB);
	cudaFree(outputArrayC);

	return 0;  
}
