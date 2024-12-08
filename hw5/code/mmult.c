#include <stdio.h>
#include <time.h>
//#include <sys/time.h> 

#define		N	1024

//struct timeval startTime;
//struct timeval finishTime;
//double timeIntervalLength;

int main(int argc, char* argv[]){

	// Define variables
	int i, j, k;
	double totalSum;
	double* inputArrayA;
	double* inputArrayB;
	double* outputArrayC;

	// Allocate arrays
	inputArrayA = (double*)malloc(N*N*sizeof(double));
	inputArrayB = (double*)malloc(N*N*sizeof(double));
	outputArrayC = (double*)malloc(N*N*sizeof(double));
	
	// Initialize arrays
	for (i = 0; i < N; i++){
		for (j = 0; j < N; j++){
			inputArrayA[i*N+j] = i*N+j;
			inputArrayB[i*N+j] = j*(N+1);
			outputArrayC[i*N+j] = 0.0;
		}
	}

	// Get the start time
	//gettimeofday(&startTime, NULL); /* START TIME */
   
	// Matrix multiplication
	for (i = 0; i < N; i++){
		for (j = 0; j < N; j++){
			for (k = 0; k < N; k++){
				outputArrayC[i*N+j] += inputArrayA[i*N+k]*inputArrayB[k*N+j];
			}
		}
	}
	
	// Get the end time
	//gettimeofday(&finishTime, NULL);

	// Check results
	totalSum = 0;
	for (i = 0; i < N; i++){
		for (j = 0; j < N; j++){
			totalSum += outputArrayC[i*N+j] - 1.0*(N*N*i + (N*(N-1))/2)*j*(N+1);
		}
	}
	printf("\nTotal Sum = %g\n", totalSum);

	//Calculate the interval length 
	//timeIntervalLength = (double)(finishTime.tv_sec-startTime.tv_sec) * 1000000 
	//                     + (double)(finishTime.tv_usec-startTime.tv_usec);
	//timeIntervalLength=timeIntervalLength/1000;

	//Print the interval length
	//printf("Interval length: %g msec.\n", timeIntervalLength);

	return 0;  
}