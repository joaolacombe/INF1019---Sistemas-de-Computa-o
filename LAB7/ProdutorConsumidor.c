#include <sys/sem.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> 
#include <sys/shm.h>
#include <sys/stat.h>
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
	
	int segment;
	char *buffer;
	int semId;

	//Cria area de memoria compartilhada para buffer
	segment = shmget (IPC_PRIVATE, sizeof(char)*BUFFER_SIZE, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
	if (segment == -1) {
		puts ("Erro na criação de segmento de memoria compartilhada."); 
		exit(1);
	}

	buffer = (char*) shmat (segment, 0, 0);

	//Fork entre produtor/consumidor
	if (fork() > 0) { //Produtor
		semId = semget (8760, 1, 0666 | IPC_CREAT);  
		setSemValue(semId); 
		while (1) {
			int i = 0;
			char c;
			//Pega semaforo
			semaforoP(semId); 
			printf("Produtor Executando area Critica\n");

			//Le até o buffer ficar cheio
			while ( i < BUFFER_SIZE) {
				buffer[i] = getchar();
				while ( (c =  getchar()) != '\n' && c != EOF);
				i++;
			}
			//Libera Semaforo
			printf("Produtor saindo area critica Buffer Lido\n");
			semaforoV(semId);
			sleep(2);
		}

	}
	else {	//Consumidor
		//Espera semaforo ser criado pelo Produtor
		while ((semId = semget (8760, 1, 0666)) < 0) sleep (1);
		while(1) {
			//Pega semaforo
			semaforoP(semId); 
			printf("Consumidor Executando area Critica\n");
			int i=0;
			//Le até o buffer ficar cheio
			while ( i < BUFFER_SIZE) {
				printf("%c", buffer[i]);
				i++;
			}
			printf("\n");
			//Libera Semaforo
			printf("COnsumidor saindo area critica, buffer escrito");
			semaforoV(semId);
			sleep(2);	
		}
	}
	
	//Area memoria compartilhada
	shmdt (buffer); 
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
