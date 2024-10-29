#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <omp.h>


#define PROBLEM_SIZE 15000


struct timeval startTime;
struct timeval finishTime;
double timeIntervalLength;

// Find first index of pid for a load-balanced partition of N items
int partition_index(long N, int pid, int nproc){
	int ratio = N/nproc;
	int reminder = N%nproc;

	if (pid < reminder){
		return (ratio+1)*pid;
	} else if (pid < nproc) { 
		return ratio*pid + reminder;
	} else {
		return N;
	}
}

int main(int argc, char* argv[])
{

	int i,j;
	double h = 0.3154;
	double*  y;
	double*  k1;
	double*  k2;
	double*  k3;
	double*  k4;
	double*  pow;
	double*  yout;
	double** c;

	double totalSum = 0.0;


	y    = (double* )malloc(PROBLEM_SIZE*sizeof(double));
	k1   = (double* )malloc(PROBLEM_SIZE*sizeof(double));
	k2   = (double* )malloc(PROBLEM_SIZE*sizeof(double));
	k3   = (double* )malloc(PROBLEM_SIZE*sizeof(double));
	k4   = (double* )malloc(PROBLEM_SIZE*sizeof(double));
	pow  = (double* )malloc(PROBLEM_SIZE*sizeof(double));
	yout = (double* )malloc(PROBLEM_SIZE*sizeof(double));
	c    = (double**)malloc(PROBLEM_SIZE*sizeof(double*));
	for (i = 0; i < PROBLEM_SIZE; i++) 
	{
		c[i] = (double*)malloc(PROBLEM_SIZE*sizeof(double));
	}

	for (i = 0; i < PROBLEM_SIZE; i++) 
	{
		y[i] = i*i;
		pow[i] = i+i;
		for (j = 0; j < PROBLEM_SIZE; j++)
		{
			c[i][j] = i*i+j;
		}
	}

	
	//Get the start time
	gettimeofday(&startTime, NULL);  /* START TIME */

	// Read number of threads
	int nproc = strtol(argv[1], NULL, 10);

    // Initialize parallel section: Calculate each k step and wait for all threads to finish between calculations.
    // Finally, reduce the total sum of the resulting vector.
    #pragma omp parallel num_threads(nproc) shared(y, k1, k2, k3, k4, pow, yout, c) private(i, j) reduction(+: totalSum)
    {
        #pragma omp for 
        for (i = 0; i < PROBLEM_SIZE; i++){ 
            k1[i] = h*pow[i];
            for (j = 0; j < PROBLEM_SIZE; j++){
                k1[i] -= h*c[i][j]*y[j];
            }
        }
        #pragma omp barrier

        #pragma omp for 
        for (i = 0; i < PROBLEM_SIZE; i++){ 
            k2[i] = h*pow[i];
            for (j = 0; j < PROBLEM_SIZE; j++){
                k2[i] -= h*c[i][j]*(y[j]+(1.0/3.0)*k1[j]);
            }
        }
        #pragma omp barrier

        #pragma omp for 
        for (i = 0; i < PROBLEM_SIZE; i++){ 
            k3[i] = h*pow[i];
            for (j = 0; j < PROBLEM_SIZE; j++){
                k3[i] -= h*c[i][j]*(y[j]+k2[j]-((1.0/3.0)*k1[j]));
            }
        }
        #pragma omp barrier

        #pragma omp for 
        for (i = 0; i < PROBLEM_SIZE; i++){ 
            k4[i] = h*pow[i];
            for (j = 0; j < PROBLEM_SIZE; j++){
                k4[i] -= h*c[i][j]*(y[j]+k1[j]-k2[j]+k3[j]);
            }

            yout[i] = y[i] + (k1[i] + 3*k2[i] + 3*k3[i] + k4[i])/8.0;
            totalSum += yout[i];
        }
    }

	//Get the end time
	gettimeofday(&finishTime, NULL);  /* END TIME */

	if (argc < 3)
		printf("\n\ntotalSum=%g\n\n",totalSum);
	
	
	//Calculate the interval length 
	timeIntervalLength = (double)(finishTime.tv_sec-startTime.tv_sec) * 1000000 
	                     + (double)(finishTime.tv_usec-startTime.tv_usec);
	timeIntervalLength=timeIntervalLength/1000;

	//Print the interval lenght
	if (argc < 3){
		printf("Interval length: %g msec.\n", timeIntervalLength);
	} else { 
		printf("%d\t%g\n", nproc, timeIntervalLength);
	}
	
	return 0;
}
