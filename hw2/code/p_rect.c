#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include <semaphore.h>

#define		NSTEPS	8388600
#define		NITER 	8388600
#define		P_START	0 
#define		P_END	10 

// Area under the curve
double area;
double *local_area;
sem_t *semaphores;

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
	local_area = malloc(nproc*sizeof(double));
	semaphores = malloc(nproc*sizeof(sem_t));
	long pid;

	for (pid = 0; pid < nproc; pid++){
		local_area[pid] = 0.0;
		sem_init(&semaphores[pid], 0, 0);
	}

	// Run parallel code
	for (pid = 0; pid < nproc; pid++)
		pthread_create(&thread_handles[pid], NULL, Pth_rect, (void *) pid);

	// Finish thread environment
	for (pid = 0; pid < nproc; pid++)
		pthread_join(thread_handles[pid] , NULL);
	free(thread_handles);
	free(local_area);
	free(semaphores);

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

	for (i = my_first_item; i < my_last_item; i++){
		local_area[my_pid] += cos(P_START+(i+0.5)*h)*h;
	}

	int stride, local_site, new_pid;
	for (stride = 1; stride < nproc; stride *= 2){
		local_site = my_pid%(2*stride);
		if (local_site == 0){
			new_pid = my_pid + stride;
			if (new_pid < nproc){
				sem_wait(&semaphores[my_pid]);
			}
		} else if (local_site == stride){
			new_pid = my_pid-stride;
			local_area[new_pid] += local_area[my_pid];
			sem_post(&semaphores[new_pid]);
		} 
	}

	if (my_pid == 0)
		area = local_area[my_pid];
	
	return NULL;
} 


