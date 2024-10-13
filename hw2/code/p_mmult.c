#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h> 
#include <pthread.h>

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

int nproc;
void *Pth_mmult (void *pid);

int main(int argc, char* argv[])
{
	int i,j;
	double totalSum;
	
	//INITIALIZE ARRAYS
	for(i = 0; i < NROW; i++)
	{
		for(j = 0; j < NCOL; j++)
		{
			inputArrayA[i][j]= i*NCOL+j;
			inputArrayB[i][j]= j*NCOL+j;
			outputArrayC[i][j]= 0;
		}
	}

	//Get the start time
	gettimeofday(&startTime, NULL); /* START TIME */

	// Initialize threads
	nproc = strtol(argv[1], NULL, 10);
	pthread_t* thread_handles;
	thread_handles = malloc(nproc*sizeof(pthread_t));
	long pid;

	// Run parallel code
	for (pid = 0; pid < nproc; pid++)
		pthread_create(&thread_handles[pid], NULL, Pth_mmult, (void *) pid);

	// Finish thread environment
	for (pid = 0; pid < nproc; pid++)
		pthread_join(thread_handles[pid] , NULL);
	free(thread_handles);
	
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

// Find first index of pid for a load-balanced partition of N items
int partition_index(long N, long pid){
	int ratio = N/nproc;
	int reminder = N%nproc;

	if (pid < reminder){
		return (ratio+1)*pid;
	} else if (pid < nproc) { 
		return ratio*pid + reminder;
	} else {
		return nproc;
	}
}

void *Pth_mmult(void *pid){
	long my_pid = (long) pid;
	int i, j, k;
	
	int my_first_row = partition_index(NROW, my_pid);
	int my_last_row = partition_index(NROW, my_pid+1);
	
	for (i = my_first_row; i < my_last_row; i++){
		for(j = 0; j < NCOL; j++){
			for(k = 0; k < NROW; k++){
				outputArrayC[i][j] += inputArrayA[i][k]*inputArrayB[k][j];
			}
		}
	}
	
	return NULL;
} 
