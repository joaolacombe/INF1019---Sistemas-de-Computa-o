#include <stdio.h>
#include <signal.h>
#include <stdlib.h> 
#include <unistd.h>
#include <sys/wait.h>
#define TEMPOEXEC 2

int main  (int argc, char *argv[]) { 
	pid_t pid1,pid2; 
	
	if ((pid1 = fork()) < 0) { 
		fprintf(stderr, "Erro ao criar filho\n"); 
		exit(-1); 
	} 
	if (pid1 == 0){ /* child 1*/
		while(1);printf("teste");}
	else/* parent */{ 
			if ((pid2 = fork()) < 0) { 
				fprintf(stderr, "Erro ao criar filho 2\n"); 
				exit(-1);
			}
			if (pid2 == 0) /* child 2*/
				while(1);
			else {
				int i;
				for (i = 0; i < 10; i++ ) {
					kill(pid1, SIGCONT);
					kill(pid2, SIGSTOP);
					printf("[1] Executando filho pid: %d.\n",pid1); 
					
					sleep(TEMPOEXEC);
					
					kill(pid1, SIGSTOP);
					kill(pid2,SIGCONT);
					printf("[2] Executando filho pid: %d.\n",pid2);
 
					sleep(TEMPOEXEC);		
				}
				kill(pid1, SIGKILL);
				kill(pid2, SIGKILL); 
				printf("Terminando execucao.\n");
			}
	} 
	return 0; 
}