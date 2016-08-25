#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main (void) {

	int fd[2];
	const char textoTX[] = "Pai, feliz dia dos pais";
	char textoRX[sizeof textoTX]; 


	pipe(fd);
	if (fork() == 0) {
		close(fd[0]);
		write(fd[1],textoTX, strlen(textoTX)+1);
	}
	else {
		close(fd[1]);
		read(fd[0], textoRX, sizeof textoRX); 
		printf("%s\n",textoRX);
	}

	return 0;
}
