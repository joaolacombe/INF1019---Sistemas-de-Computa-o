#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/stat.h>
#define MEM_KEY 8752
#define MAX_LENGTH 141

int main (int argc,char *argv[]) {

	int segment=0; 
	char *mday;

	segment = shmget (MEM_KEY, sizeof(char)*MAX_LENGTH, S_IRUSR |  S_IWUSR);
	if (segment == -1) {
		printf("Erro segment.\n");
		exit(1);
	}
	mday = (char *) shmat (segment, 0, 0);

	printf("Mensagem do dia: %s\n",mday);

	shmdt (mday); 
	shmctl (segment, IPC_RMID, 0); 

	return 0;	

}