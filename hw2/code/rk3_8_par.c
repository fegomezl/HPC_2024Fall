#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include <semaphore.h>


#define PROBLEM_SIZE 2048
#define h 0.3154

// Runge-Kutta variables variables
double y[PROBLEM_SIZE];
double poww[PROBLEM_SIZE];
double yout[PROBLEM_SIZE];
double c[PROBLEM_SIZE][PROBLEM_SIZE];
double k1[PROBLEM_SIZE];
double k2[PROBLEM_SIZE];
double k3[PROBLEM_SIZE];
double k4[PROBLEM_SIZE];

// pthreads variables
int barrier_counter;
pthread_mutex_t barrier_mutex;
pthread_cond_t barrier_condvar;
double totalSum;

struct timeval startTime;
struct timeval finishTime;
double timeIntervalLength;

int nproc;
void *Pth_rk (void *pid);

int main(int argc, char* argv[])
{
	int i,j;

	barrier_counter = 0;
	totalSum = 0.0;

	for (i = 0; i < PROBLEM_SIZE; i++) 
	{
		y[i]=i*i;
		poww[i]=i+i;
		for (j = 0; j < PROBLEM_SIZE; j++)
		{
			c[i][j]=i*i+j;
		}
	}

	
	//Get the start time
	gettimeofday(&startTime, NULL);  /* START TIME */

	// Initialize threads
	nproc = strtol(argv[1], NULL, 10);
	pthread_t* thread_handles;
	thread_handles = malloc(nproc*sizeof(pthread_t));
	pthread_mutex_init(&barrier_mutex, NULL);
	pthread_cond_init(&barrier_condvar, NULL);
	long pid;

	// Run parallel code
	for (pid = 0; pid < nproc; pid++)
		pthread_create(&thread_handles[pid], NULL, Pth_rk, (void *) pid);

	// Finish thread environment
	for (pid = 0; pid < nproc; pid++)
		pthread_join(thread_handles[pid] , NULL);
	pthread_mutex_destroy(&barrier_mutex);
	pthread_cond_destroy(&barrier_condvar);
	free(thread_handles);

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

void barrier(){
	pthread_mutex_lock(&barrier_mutex);
	barrier_counter++;
	if (barrier_counter == nproc){
		barrier_counter = 0;
		pthread_cond_broadcast(&barrier_condvar);
	} else {
		while (pthread_cond_wait(&barrier_condvar, &barrier_mutex) != 0);
	}
	pthread_mutex_unlock(&barrier_mutex);
}

void *Pth_rk(void *pid){
	long my_pid = (long) pid;
	
	int i, j;
    double local_sum = 0.0;
	int my_first_row = partition_index(PROBLEM_SIZE, my_pid);
	int my_last_row = partition_index(PROBLEM_SIZE, my_pid+1);

	for (i = my_first_row; i < my_last_row; i++){ 
		k1[i] = h*poww[i];
		for (j = 0; j < PROBLEM_SIZE; j++){
			k1[i] -= h*c[i][j]*y[j];
		}
	}

	barrier();

	for (i = my_first_row; i < my_last_row; i++){ 
		k2[i] = h*poww[i];
		for (j = 0; j < PROBLEM_SIZE; j++){
			k2[i] -= h*c[i][j]*(y[j]+(1.0/3.0)*k1[j]);
		}
	}

	barrier();

	for (i = my_first_row; i < my_last_row; i++){ 
		k3[i] = h*poww[i];
		for (j = 0; j < PROBLEM_SIZE; j++){
			k3[i] -= h*c[i][j]*(y[j]+k2[j]-((1.0/3.0)*k1[j]));
		}
	}

	barrier();

	for (i = my_first_row; i < my_last_row; i++){ 
		k4[i] = h*poww[i];
		for (j = 0; j < PROBLEM_SIZE; j++){
			k4[i] -= h*c[i][j]*(y[j]+k1[j]-k2[j]+k3[j]);
		}

		yout[i] = y[i] + (k1[i] + 3*k2[i] + 3*k3[i] + k4[i])/8.0;
        local_sum += yout[i];
	}

	pthread_mutex_lock(&barrier_mutex);
    totalSum += local_sum; 
	pthread_mutex_unlock(&barrier_mutex);
	
	return NULL;
} 
