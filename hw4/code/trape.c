#include <stdio.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>

#define		NSTEPS	8388608
#define		NITER 	8388608
#define		P_START	0 
#define		P_END	10 


struct timeval startTime;
struct timeval finishTime;
double timeIntervalLength;


int main(int argc, char* argv[])
{

	int i;
	double h;
	double area;
	double p_a = P_START;
	double p_b;
	double f_a,f_b;

	

		//
		//I N I T I A L I Z A T I O N S
		//
		h=(double)(P_END-P_START)/NSTEPS;
		p_a = P_START;
		p_b = p_a+h;
		area=0.0;


		//Get the start time
		gettimeofday(&startTime, NULL);


		for(i = 0; i < NITER; i++)
			{
				p_a = i*h;
				p_b = p_a + h;
				f_a = sin(p_a);
				f_b = sin(p_b);
				area += 0.5*h*(f_a + f_b);
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
