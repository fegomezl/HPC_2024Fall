#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h> 
#include <omp.h> 

#define		NROW	1024
#define		NCOL	NROW


#define TEST_RESULTS


//Input Array A
int inputArrayA  [NROW][NCOL];
//Input Array B
int inputArrayB  [NROW][NCOL];
//Output Array C
int outputArrayC [NROW][NCOL];

struct timeval startTime;
struct timeval finishTime;
double timeIntervalLength;

int main(int argc, char* argv[])
{
	int i,j;
	double totalSum;
	
	//INITIALIZE ARRAYS
	for(i = 0; i < NROW; i++)
	{
		for(j = 0; j < NCOL; j++)
		{
			inputArrayA[i][j] = i*NCOL + j;
			inputArrayB[i][j] = j*NCOL + j;
			outputArrayC[i][j] = 0;
		}
	}

	//Get the start time
	gettimeofday(&startTime, NULL); /* START TIME */

	// Read number of threads
    int nproc = strtol(argv[1], NULL, 10);

    // Divide rows and columns of resulting matrix among threads
    int n, ii, jj, kk;
    #pragma omp parallel for num_threads(nproc) shared(inputArrayA, inputArrayB, outputArrayC) private(n, ii, jj, kk)
    for (n = 0; n < NCOL*NROW; n++){
        ii = n/NCOL;
        jj = n%NCOL;
        for (kk = 0; kk < NROW; kk++)
            outputArrayC[ii][jj] += inputArrayA[ii][kk]*inputArrayB[kk][jj];
    }
	
	//Get the end time
	gettimeofday(&finishTime, NULL);  /* END TIME */

	#ifdef TEST_RESULTS
		//CALCULATE TOTAL SUM 
		//[Just for verification]
		totalSum=0;
		//
		for(i=0;i<NROW;i++)
		{
			for(j=0;j<NCOL;j++)
			{
				totalSum+=(double)outputArrayC[i][j];
			}
		}
	
		printf("\nTotal Sum = %g\n",totalSum);
	#endif

	//Calculate the interval length 
	timeIntervalLength = (double)(finishTime.tv_sec-startTime.tv_sec) * 1000000 
	                     + (double)(finishTime.tv_usec-startTime.tv_usec);
	timeIntervalLength=timeIntervalLength/1000;

	//Print the interval length
	printf("Interval length: %g msec.\n", timeIntervalLength);

	return 0;  
}
