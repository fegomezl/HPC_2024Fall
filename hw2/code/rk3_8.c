#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>


#define PROBLEM_SIZE 8192


struct timeval startTime;
struct timeval finishTime;
double timeIntervalLength;


int main(int argc, char* argv[])
{
	int i,j;
	double h=0.3154;
	double*  y;
	double*  yt;
	double*  k1;
	double*  k2;
	double*  k3;
	double*  k4;
	double*  pow;
	double*  yout;
	double** c;

	double totalSum=0.0;


	y    = (double* )malloc(PROBLEM_SIZE*sizeof(double));
	yt   = (double* )malloc(PROBLEM_SIZE*sizeof(double));
	k1   = (double* )malloc(PROBLEM_SIZE*sizeof(double));
	k2   = (double* )malloc(PROBLEM_SIZE*sizeof(double));
	k3   = (double* )malloc(PROBLEM_SIZE*sizeof(double));
	k4   = (double* )malloc(PROBLEM_SIZE*sizeof(double));
	pow  = (double* )malloc(PROBLEM_SIZE*sizeof(double));
	yout = (double* )malloc(PROBLEM_SIZE*sizeof(double));
	c    = (double**)malloc(PROBLEM_SIZE*sizeof(double*));
	for (i=0;i<PROBLEM_SIZE;i++) 
	{
		c[i]=(double*)malloc(PROBLEM_SIZE*sizeof(double));
	}

	for (i = 0; i < PROBLEM_SIZE; i++) 
	{
		y[i]=i*i;
		pow[i]=i+i;
		for (j = 0; j < PROBLEM_SIZE; j++)
		{
			c[i][j]=i*i+j;
		}
	}

	
	//Get the start time
	gettimeofday(&startTime, NULL);  /* START TIME */

	for (i = 0; i < PROBLEM_SIZE; i++) 
	{ 
		yt[i] = 0.0;
		for (j = 0; j < PROBLEM_SIZE; j++) 
		{
			yt[i] += c[i][j]*y[j];
		}
		k1[i] = h*(pow[i]-yt[i]);
	}

	for (i = 0; i < PROBLEM_SIZE; i++) 
	{
		yt[i] = 0.0;
		for (j = 0; j < PROBLEM_SIZE; j++) 
		{
			yt[i] += c[i][j]*(y[j]+(1.0/3.0)*k1[j]);
		}
		k2[i] = h*(pow[i]-yt[i]);
	}

	for (i = 0; i < PROBLEM_SIZE; i++) 
	{
		yt[i] = 0.0;
		for (j = 0; j < PROBLEM_SIZE; j++) 
		{
			yt[i] += c[i][j]*(y[j]+k2[j]-((1.0/3.0)*k1[j]));
		}
		k3[i] = h*(pow[i]-yt[i]);
	}

	for (i =0; i < PROBLEM_SIZE; i++) 
	{
		yt[i]=0.0;
		for (j = 0; j < PROBLEM_SIZE; j++) 
		{
			yt[i] += c[i][j]*(y[j]+k1[j]-k2[j]+k3[j]);
		}
		k4[i] = h*(pow[i]-yt[i]);

		yout[i] = y[i] + (k1[i] + 3*k2[i] + 3*k3[i] + k4[i])/8.0;
		totalSum+=yout[i];
	}

	//Get the end time
	gettimeofday(&finishTime, NULL);  /* END TIME */

	printf("\n\ntotalSum=%g\n\n",totalSum);
	
	
	//Calculate the interval length 
	timeIntervalLength = (double)(finishTime.tv_sec-startTime.tv_sec) * 1000000 
	                     + (double)(finishTime.tv_usec-startTime.tv_usec);
	timeIntervalLength=timeIntervalLength/1000;

	//Print the interval length
	printf("Interval length: %g msec.\n", timeIntervalLength);
	
	

	
	return 0;
}
