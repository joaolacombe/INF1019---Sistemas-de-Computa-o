#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

int main () {

	int filho_pid;
	int valor = 1;
	
	printf("Valor: %d\n",valor);
	filho_pid = fork();
	
	if (filho_pid != 0 ) { 
		int status;
		waitpid(filho_pid,&status,0);
		printf("Valor no processo pai: %d\n",valor);
}
	else {
		valor = 5;
		printf("Valor no processo filho: %d\n",valor);
		exit(1);
}
	return 0;
}
