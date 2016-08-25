#include <sys/sem.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> 
#include <sys/shm.h>
#include <sys/stat.h>
#define BUFFER_SIZE 16
#define MY_IPC 1212

union  semun { 
	int val; 
	struct semid_ds *buf; 
	ushort *array; 
};

// inicializa o valor do semáforo
int setSemValue(int semId);
// remove o semáforo
void delSemValue(int semId);
// operação P
int semaforoP(int semId);
//operação V
int semaforoV(int semId);



int main (int argc, char * argv[]) { 
	
	int segment;
	int *global;
	int semId;

	//Cria area de memoria compartilhada para buffer
	segment = shmget (MY_IPC, sizeof(int), IPC_CREAT | S_IRUSR | S_IWUSR);
	if (segment == -1) {
		puts ("Erro na criação de segmento de memoria compartilhada."); 
		exit(1);
	}

	global = (int*) shmat (segment, 0, 0);
	*global = 0;

	semId = semget (8765	, 1, 0666 | IPC_CREAT);  
	setSemValue(semId); 
	while (1) {
		semaforoP(semId); 
		*global = (*global) + 1;
		printf("Somado 1: %d\n",*global);
		sleep(rand()%3);	
		semaforoV(semId);
		sleep(rand()%2);	
	}
	
	shmdt (global); 
	shmctl (segment, IPC_RMID, 0); 

	return 0; 
}

int setSemValue(int semId) { 
	union semun semUnion; 
	semUnion.val = 1; 
	return semctl(semId, 0, SETVAL, semUnion); 
}

void delSemValue(int semId) { 
	union semun semUnion; 
	semctl(semId, 0, IPC_RMID, semUnion); 
}

int semaforoP(int semId) { 
	struct sembuf semB; 
	semB.sem_num = 0; 
	semB.sem_op = -1; 
	semB.sem_flg = SEM_UNDO; 
	semop(semId, &semB, 1); 
	return 0; 
}

int semaforoV(int semId) { 
	struct sembuf semB; 
	
	semB.sem_num = 0; 
	semB.sem_op = 1; 
	semB.sem_flg = SEM_UNDO; 
	semop(semId, &semB, 1); 

	return 0; 
} 
