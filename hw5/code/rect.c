#include <stdio.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>

// System size
#define		N	1048576

// Structures for profilling
struct timeval startTime;
struct timeval finishTime;
double timeIntervalLength;

int main(int argc, char* argv[]){

	// Define variables
	int i;
	double h;
	double area;
	
	// Initialize variables
	h = 10.0/N;
	area = 0.0;

	// Get the start time
	gettimeofday(&startTime, NULL);

	// Execute sum
	for(i = 0; i < N; i++)
		area += cos(i*h)*h;

	// Get the end time
	gettimeofday(&finishTime, NULL);
	
	// Check results
	if (argc < 2)
		printf("Result : %.2lf \n", area);
	
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
