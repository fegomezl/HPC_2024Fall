#include <stdio.h>
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
	int i, j, k, n;
	double totalSum, expectedSum;
	double* inputArrayA;
	double* inputArrayB;
	double* outputArrayC;

	// Allocate variables
	inputArrayA = (double*)malloc(N*N*sizeof(double));
	inputArrayB = (double*)malloc(N*N*sizeof(double));
	outputArrayC = (double*)malloc(N*N*sizeof(double));
	
	// Initialize variables
    nproc = strtol(argv[1], NULL, 10);
	for (i = 0; i < N; i++){
		for (j = 0; j < N; j++){
			inputArrayA[i*N+j] = i*N+j;
			inputArrayB[i*N+j] = j*(N+1);
			outputArrayC[i*N+j] = 0.0;
		}
	}

	// Get the start time
	gettimeofday(&startTime, NULL);

    // Divide rows and columns of resulting matrix among threads
    #pragma omp parallel for num_threads(nproc) shared(inputArrayA, inputArrayB, outputArrayC) private(n, i, j, k)
    for (n = 0; n < N*N; n++){
        i = n/N;
        j = n%N;
        for (k = 0; k < N; k++)
            outputArrayC[n] += inputArrayA[i*N+k]*inputArrayB[k*N+j];
    }
	
	// Get the end time
	gettimeofday(&finishTime, NULL);

	// Check results
	if (argc < 3){
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

	// Calculate the interval length 
	timeIntervalLength = (double)(finishTime.tv_sec-startTime.tv_sec)*1000000 
	                   + (double)(finishTime.tv_usec-startTime.tv_usec);
	timeIntervalLength = timeIntervalLength/1000;

	// Print the interval lenght
	if (argc < 3){
		printf("Interval length: %g msec.\n", timeIntervalLength);
	} else { 
		printf("%d\t%g\n", nproc, timeIntervalLength);
	}

	return 0;  
}