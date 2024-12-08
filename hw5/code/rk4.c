#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

// System size
#define		N	256

// Structures for profilling
struct timeval startTime;
struct timeval finishTime;
double timeIntervalLength;

int main(int argc, char* argv[]){

	// Define variables
	int i, j;
	double h, totalSum;
	double*  y;
	double*  k1;
	double*  k2;
	double*  k3;
	double*  k4;
	double*  pow;
	double*  yout;
	double* c;
	
	// Allocate variables
	y    = (double*)malloc(N*sizeof(double));
	k1   = (double*)malloc(N*sizeof(double));
	k2   = (double*)malloc(N*sizeof(double));
	k3   = (double*)malloc(N*sizeof(double));
	k4   = (double*)malloc(N*sizeof(double));
	pow  = (double*)malloc(N*sizeof(double));
	yout = (double*)malloc(N*sizeof(double));
	c    = (double*)malloc(N*N*sizeof(double));

	// Initialize variables
	h = 0.3154;
	totalSum = 0.0;
	for (i = 0; i < N; i++){
		y[i] = i*i;
		pow[i] = 2*i;
		for (j = 0; j < N; j++)
			c[i*N+j] = i*i+j;
	}

	// Get the start time
	gettimeofday(&startTime, NULL);

	// Apply RK4 method, calculating each step one at a time
	for (i = 0; i < N; i++){ 
        	k1[i] = pow[i];
        	for (j = 0; j < N; j++)
            		k1[i] -= c[i*N+j]*y[j];
		k1[i] *= h;
	} 	

	for (i = 0; i < N; i++){ 
        	k2[i] = pow[i];
        	for (j = 0; j < N; j++)
            		k2[i] -= c[i*N+j]*(y[j]+0.5*k1[j]);
		k2[i] *= h;
	}

	for (i = 0; i < N; i++){ 
        	k3[i] = pow[i];
        	for (j = 0; j < N; j++)
            		k3[i] -= c[i*N+j]*(y[j]+0.5*k2[j]);
		k3[i] *= h;
	}

	for (i = 0; i < N; i++){ 
        	k4[i] = pow[i];
        	for (j = 0; j < N; j++)
            		k4[i] -= c[i*N+j]*(y[j]+k3[j]);
		k4[i] *= h;

		yout[i] = y[i] + (k1[i] + 2*k2[i] + 2*k3[i] + k4[i])/6.0;
	}

	// Get the end time
	gettimeofday(&finishTime, NULL);

	// Check results
	if (argc < 2){
		for (i = 0; i < N; i++)
			totalSum += yout[i];
		printf("Total Sum : %g \n", totalSum);
	}
	
	// Calculate the interval length 
	timeIntervalLength = (double)(finishTime.tv_sec-startTime.tv_sec)*1000000 
	                   + (double)(finishTime.tv_usec-startTime.tv_usec);
	timeIntervalLength = timeIntervalLength/1000;

	// Print the interval length
	if (argc < 2){
		printf("Interval length: %g msec.\n", timeIntervalLength);
	} else { 
		printf("%g\n", timeIntervalLength);
	}

	return 0;
}
