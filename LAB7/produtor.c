#include <sys/sem.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> 
#define BUFFER_SIZE 16

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
	int i; 
	char letra ; 

	


	//Cria semaforo

	//Cria area de memoria compartilhada para buffer

	//Fork entre produtor/consumidor

	if (fork() == 0) { //Produtor
		int semId = semget (8752, 1, 0666 | IPC_CREAT);  
		while (1) {
			//Pega semaforo
			semaforoP(semId); 
			int i;
			//Le até o buffer ficar cheio
			while ( i < BUFFER_SIZE) {
				char c;
				c = getchar();
				//Escreve no Buffer
			}
			//Libera Semaforo
			semaforoV(semId);
		}

	}
	else {	//Consumidor
		//Espera semaforo ser criado pelo Produtor
		while ((semId = semget (8752, 1, 0666)) < 0) sleep (1);  
		while(1) {
			//Pega semaforo
			semaforoP(semId); 
			int i;
			//Le até o buffer ficar cheio
			while ( i < BUFFER_SIZE) {
				char c;
				//Le do buffer
				//Escreve no Buffer
				putchar (c); fflush(stdout); 
			}
			//Libera Semaforo
			semaforoV(semId);
		}
	}
	
	//Libera semaforo/areas memoria compartilhada

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
