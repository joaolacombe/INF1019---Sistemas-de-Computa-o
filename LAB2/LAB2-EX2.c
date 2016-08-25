#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <string.h>
#define MEM_KEY 8752
#define MAX_LENGTH 141

int main (int argc,char *argv[]) {

	int segment=0; 
	char msg[MAX_LENGTH],*mday;

	printf("Insira mensagem do dia\n");
	scanf(" %140[^\n]",msg);

	segment = shmget (MEM_KEY, sizeof(char)*MAX_LENGTH, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
	if (segment == -1) {
		puts ("Erro na criação de segmento de memoria compartilhada.");
		exit(1);
	}
	mday = (char *) shmat (segment, 0, 0);
	strcpy(mday,msg);

	shmdt (mday); 

	return 0;
}