#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>


//
int main () {
	
	int filho_pid = fork();
	
	if (filho_pid != 0 ) { 
		int status;
		printf("Pai pid: %d\n", getpid());
		waitpid(filho_pid,&status,0);
	}
	else {
		printf("Filho pid: %d\n", getpid());
	}
	return 0;
}
