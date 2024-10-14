#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>


#define PROBLEM_SIZE 8192

// Area under the curve
int barrier_counter;
pthread_mutex_t barrier_mutex;
pthread_cond_t barrier_condvar;
double totalSum;
double *local_sum;
sem_t *semaphores;

struct timeval startTime;
struct timeval finishTime;
double timeIntervalLength;

int nproc;
void *Pth_rk (void *pid);

int main(int argc, char* argv[])
{
	int i,j;
	double h = 0.3154;
	double*  y;
	double*  yt;
	double*  k1;
	double*  k2;
	double*  k3;
	double*  k4;
	double*  pow;
	double*  yout;
	double** c;

	barrier_counter = 0;
	totalSum = 0.0;

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

	// Initialize threads
	nproc = strtol(argv[1], NULL, 10);
	pthread_t* thread_handles;
	thread_handles = malloc(nproc*sizeof(pthread_t));
	pthread_mutex_init(&barrier_mutex, NULL);
	pthread_cond_init(&barrier_condvar, NULL);
	local_sum = malloc(nproc*sizeof(double));
	semaphores = malloc(nproc*sizeof(sem_t));
	long pid;

	for (pid ii = 0; pid < nproc; ii++){
		local_sum[ii] = 0.0;
		sem_init(&semaphores[ii], 0, 0);
	}

	// Run parallel code
	for (pid = 0; pid < nproc; pid++)
		pthread_create(&thread_handles[pid], NULL, Pth_rk, (void *) pid);

	// Finish thread environment
	for (pid = 0; pid < nproc; pid++)
		pthread_join(thread_handles[pid] , NULL);
	free(thread_handles);
	pthread_mutex_destroy(barrier_mutex);
	pthread_cond_destroy(barrier_condvar);
	free(local_area);
	free(semaphores);

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
	counter++;
	if (counter == nproc){
		counter = 0;
		pthread_cond_broadcast(&barrier_condvar);
	} else {
		while (pthread_cond_wait(&barrier_condvar, &barrier_mutex) != 0);
	}
	pthread_mutex_unlock(&mutex);
}

void *Pth_rk(void *pid){
	long my_pid = (long) pid;
	
	int i, j;
	int my_first_row = partition_index(NROW, my_pid);
	int my_last_row = partition_index(NROW, my_pid+1);

	for (i = my_first_row; i < my_last_row; i++){ 
		k1[i] = h*pow[i];
		for (j = 0; j < PROBLEM_SIZE; j++){
			k1[i] -= h*c[i][j]*y[j];
		}
	}

	barrier();

	for (i = my_first_row; i < my_last_row; i++){ 
		k2[i] = h*pow[i];
		for (j = 0; j < PROBLEM_SIZE; j++){
			k2[i] -= h*c[i][j]*(y[j]+(1.0/3.0)*k1[j]);
		}
	}

	barrier();

	for (i = my_first_row; i < my_last_row; i++){ 
		k3[i] = h*pow[i];
		for (j = 0; j < PROBLEM_SIZE; j++){
			k3[i] -= h*c[i][j]*(y[j]+k2[j]-((1.0/3.0)*k1[j]));
		}
	}

	barrier();

	for (i = my_first_row; i < my_last_row; i++){ 
		k4[i] = h*pow[i];
		for (j = 0; j < PROBLEM_SIZE; j++){
			k4[i] -= h*c[i][j]*(y[j]+k1[j]-k2[j]+k3[j]);
		}

		yout[i] = y[i] + (k1[i] + 3*k2[i] + 3*k3[i] + k4[i])/8.0;
		local_sum[my_pid] += yout[i];
	}

	int local_site, new_pid;
	for (int stride = 1; stride < n; stride *= 2){
		local_site = my_pid%(2*stride);
		if (local_site = stride){
			new_pid = my_pid-stride;
			local_sum[new_pid] += local_sum[my_pid];
			sem_post(&semaphores[new_pid]);
		} else if (local_site == 0){
			new_pid = my_pid + stride;
			if (new_pid < nproc){
				sem_wait(&semaphores[new_pid]);
			}
		} 
	}

	if (my_pid == 0)
		totalSum = local_sum[my_pid];
	
	return NULL;
} 