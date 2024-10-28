#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include <semaphore.h>

#define		NSTEPS	1000000000
#define		NITER 	1000000000
#define		P_START	0 
#define		P_END	10 

// Area under the curve
double area;
pthread_mutex_t barrier_mutex;

struct timeval startTime;
struct timeval finishTime;
double timeIntervalLength;

int nproc;
void *Pth_rect (void *pid);

int main(int argc, char* argv[]){

	//
	//I N I T I A L I Z A T I O N S
	//
	area = 0.0;

	//Get the start time
	gettimeofday(&startTime, NULL);

	// Initialize threads
	nproc = strtol(argv[1], NULL, 10);
	pthread_t* thread_handles;
	thread_handles = malloc(nproc*sizeof(pthread_t));
	long pid;

	pthread_mutex_init(&barrier_mutex, NULL);

	// Run parallel code
	for (pid = 0; pid < nproc; pid++)
		pthread_create(&thread_handles[pid], NULL, Pth_rect, (void *) pid);

	// Finish thread environment
	for (pid = 0; pid < nproc; pid++)
		pthread_join(thread_handles[pid] , NULL);
	free(thread_handles);

	//Get the end time
	gettimeofday(&finishTime, NULL);  /* after time */
		
	printf("Result : %.2lf \n", area);
	
	//Calculate the interval length 
	timeIntervalLength = (double)(finishTime.tv_sec-startTime.tv_sec) * 1000000 
		     + (double)(finishTime.tv_usec-startTime.tv_usec);
	timeIntervalLength = timeIntervalLength/1000;

	//Print the interval lenght
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
		return N;
	}
}

void *Pth_rect(void *pid){
	long my_pid = (long) pid;

	double h = (double)(P_END-P_START)/NSTEPS;
	
	int i;
	int my_first_item = partition_index(NSTEPS, my_pid);
	int my_last_item = partition_index(NSTEPS, my_pid+1);

    double local_area=0.0;
	for (i = my_first_item; i < my_last_item; i++){
        local_area += cos(P_START+(i+0.5)*h)*h;
	}

	pthread_mutex_lock(&barrier_mutex);
    area += local_area; 
	pthread_mutex_unlock(&barrier_mutex);
	
	return NULL;
} 


