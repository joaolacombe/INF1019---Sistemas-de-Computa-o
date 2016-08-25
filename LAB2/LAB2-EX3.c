#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#define PROC 50

void verifica_erro(int id) {
	if ( id < 0) { 
		puts ("Erro na criação do novo processo"); 
		exit (-2); 
	}
}

int main () {

	int *vet, n = 9, vaux[9] = {30,10,90,40,60,50,70,20,80};
	int segment, id;
	int ini = 0,i;

	segment = shmget (IPC_PRIVATE, sizeof(int)*n, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
	if (segment == -1) {
	puts ("Erro na criação de segmento de memoria compartilhada."); 
		exit(1);
	}

	vet = (int*) shmat (segment, 0, 0);
	for (i = 0; i < n; i++) {
		vet[i] = vaux[i];
	}

	id = fork();
	verifica_erro(id);
	if (id != 0) {
		ini += 3;
		id = fork();
		verifica_erro(id);
		if ( id != 0) {
			ini+=3;
			id = fork();
			verifica_erro(id);
		}
	}
	if (id == 0) {
		for(i=0; i < 3; i++) {	
			if(vet[i+ini] == PROC) {
				printf("Procurado esta na posicao %d\n", i+ini);
			}
		}
	}

	shmdt (vet); 
	shmctl (segment, IPC_RMID, 0); 

	return 0;
}