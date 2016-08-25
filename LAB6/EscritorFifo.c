#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define OPENMODE O_WRONLY
#define TRUE 1

int main (void) {
	int fifo;
	char c[100];

	if ((fifo = open("minhaFifo", OPENMODE)) < 0) { 
		puts ("Erro ao abrir a FIFO para escrita"); 
		return -1; 
	} 

	while (TRUE) {
		//scanf("%s", c);
		gets(c);
		if(strcmp("PARA",c) == 0) break;
		write(fifo, c, strlen(c));  
	}
	close (fifo);
	return 0;
}
