#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define NUM_THREADS 2

static int iglobal = 0 ;

void *contCrescente (void *threadid)  {
	int i;
	for (i = 1; i <=20; i++) {
		printf("%d\n",iglobal++);
		sleep(1);
	}
        pthread_exit(NULL); 
}
void *contDecrescente (void *threadid)  {
	int i;
	for (i = 30; i >=1; i--) {
		printf("%d\n",iglobal++);
		sleep(2);
	}
	pthread_exit(NULL);
}

int main (void) {

	pthread_t threads[NUM_THREADS];  

	pthread_create(&threads[0], NULL, contCrescente, (void *)0); 
	pthread_create(&threads[1], NULL, contDecrescente, (void *)1); 

	pthread_join(threads[0],NULL);
	pthread_join(threads[1],NULL);

	return 0;
}
