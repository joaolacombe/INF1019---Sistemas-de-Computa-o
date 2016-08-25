#include <stdio.h>
#include <signal.h>
#include <stdlib.h> 
#include <unistd.h>
#include <sys/wait.h>



int main  (int argc, char *argv[]) { 
	pid_t pid1,pid2,pid3; 
	char *p1,*p2,*p3;

	p1 = argv[1];
	p2 = argv[2];
	p3 = argv[3];
	printf("%s %s %s\n",p1,p2,p3);


	if ((pid1 = fork()) < 0) { 
		fprintf(stderr, "Erro ao criar filho\n"); 
		exit(-1); 
	} 
	if (pid1 == 0) {
		char *const args[] = {p1, 0};		
		execv(p1,args);
	}
	else{ 
		kill(pid1,SIGSTOP);
		if ((pid2 = fork()) < 0) { 
			fprintf(stderr, "Erro ao criar filho\n"); 
			exit(-1); 
		} 
		if (pid2 == 0) {
			char *const args[] = {p2, 0};
			execv(p2,args);
		}
		else {
			kill(pid2,SIGSTOP);
			if ((pid3 = fork()) < 0) { 
				fprintf(stderr, "Erro ao criar filho\n"); 
				exit(-1); 
			} 
			if (pid3 == 0) {
				char *const args[] = {p3, 0};	
				execv(p3,args);
			}
			else {
				kill(pid3,SIGSTOP);
				while(1) {
					kill(pid1,SIGCONT);
					kill(pid3,SIGSTOP);
					sleep(1);
					kill(pid1,SIGSTOP);
					kill(pid2,SIGCONT);
					sleep(2);
					kill(pid2,SIGSTOP);
					kill(pid3,SIGCONT);
					sleep(2);
		
				}


			}





		}

	} 

	return 0; 
}
