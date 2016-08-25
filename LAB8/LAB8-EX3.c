#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#define NUM_THREADS 2
#define N 8

struct fila {
	int n;
	int ini;
	int vet[N];
};
typedef struct fila Fila;


static Fila *f;


Fila* fila_cria (void) {
	Fila* f = (Fila*) malloc(sizeof(Fila));
	f->n = 0;     
	f->ini = 0;
	return f;
}
int fila_vazia(Fila *f) {
	if (f->n == 0)
		return 1;
	return 0;
}

void fila_insere (Fila* f, int v) {
	int fim;
	if (f->n == N) {   
		/* fila cheia: capacidade esgotada */
		printf("Capacidade da fila estourou.\n");
		exit(1);         
		/* aborta programa */
	}
	/* insere elemento na próxima posição livre */
	fim = (f->ini + f->n) % N;
	f->vet[fim] = v;
	f->n++;
}
int fila_retira (Fila* f){ 
	int v;
	if (fila_vazia(f)) {
		printf("Fila vazia.\n");
		exit(1);         
		/* aborta programa */
	}
	/* retira elemento do início */
	v = f->vet[f->ini];
	f->ini = (f->ini + 1) % N;
	f->n--;
	return v;
}

void *produtor (void *threadid)  {
	int i;

	for (i = 1; i <=64;) {
		int random;
		srand(time(NULL));
		random = rand() % 64;
		if (f->n >= N) {
			sleep(1);
			continue;
		}
		printf("PRODUZI: %d\n",random);
		fila_insere(f,random);
		sleep(1);
		i++;
		
	}
        pthread_exit(NULL); 
}
void *consumidor (void *threadid)  {
	int i;
	for (i = 30; i >=1;) {
		if (f->n == 0) {
			sleep(2);
			continue;		
		}
		printf("CONSUMI:%d\n",fila_retira(f));
		sleep(2);
		i--;
	}
	pthread_exit(NULL);
}



int main (void) {

	pthread_t threads[NUM_THREADS];  
	f = fila_cria();
	pthread_create(&threads[0], NULL, produtor, (void *)0); 
	pthread_create(&threads[1], NULL, consumidor, (void *)1); 

	pthread_join(threads[0],NULL);
	pthread_join(threads[1],NULL);


	return 0;
}
