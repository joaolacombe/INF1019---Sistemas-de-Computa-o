#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int main () {

	int filho_pid;
	
	filho_pid = fork();
	
	if (filho_pid != 0 ) { 
		int status;
		waitpid(filho_pid,&status,0);
	}
	else {
		char *envp[] = { NULL };
		char *argv[] = { "./LAB1-EX1",NULL };
		execve("LAB1-EX1",argv,envp);
	}
	return 0;
}