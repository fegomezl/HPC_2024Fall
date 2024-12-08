#include <stdio.h>
#include <math.h>
#include <time.h>
//#include <sys/time.h>

#define		NSTEPS	1048576
#define		P_START	0 
#define		P_END	10 

//struct timeval startTime;
//struct timeval finishTime;
//double timeIntervalLength;

int main(int argc, char* argv[]){

	// Define variables
	int i;
	double h;
	double area;
	
	// Initialize variables
	h = (double)(P_END-P_START)/NSTEPS;
	area = 0.0;

	// Get the start time
	//gettimeofday(&startTime, NULL);

	// Execute sum
	for(i = 0; i < NSTEPS; i++)
		area += cos(P_START+i*h)*h;

	// Get the end time
	//gettimeofday(&finishTime, NULL);
	
	// Check results
	printf("Result : %.2lf \n",area);
	
	// Calculate the interval length 
	//timeIntervalLength = (double)(finishTime.tv_sec-startTime.tv_sec) * 1000000 
	//	     		   + (double)(finishTime.tv_usec-startTime.tv_usec);
	//timeIntervalLength = timeIntervalLength/1000;

	// Print the interval lenght
	//printf("Interval length: %g msec.\n", timeIntervalLength);
        
	return 0;
}
