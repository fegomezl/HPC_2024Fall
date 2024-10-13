#include <stdio.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>

#define		NSTEPS	8388600
#define		NITER 	8388600
#define		P_START	0 
#define		P_END	10 


struct timeval startTime;
struct timeval finishTime;
double timeIntervalLength;

int NUM_THREADS = 8;

int main(int argc, char* argv[])
{

	int i;
	double h;
	double area;
	double p_current = P_START;
	double f_result;

	//
	//I N I T I A L I Z A T I O N S
	//
	h = (double)(P_END-P_START)/NSTEPS;
	p_current = P_START;
	area=0.0;


	//Get the start time
	gettimeofday(&startTime, NULL);


	for(i = 0; i < NITER; i++)
	{
		p_current = i*h;
		f_result = cos(p_current);
		area += f_result*h;
		p_current += h;
	}

	//Get the end time
	gettimeofday(&finishTime, NULL);  /* after time */
	
	
	printf("Result : %.2lf \n",area);
	
	//Calculate the interval length 
	timeIntervalLength = (double)(finishTime.tv_sec-startTime.tv_sec) * 1000000 
		     + (double)(finishTime.tv_usec-startTime.tv_usec);
	timeIntervalLength=timeIntervalLength/1000;

	//Print the interval lenght
	printf("Interval length: %g msec.\n", timeIntervalLength);

        
	return 0;
}
