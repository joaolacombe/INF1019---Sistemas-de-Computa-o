#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#define OPENMODE O_RDONLY
#define TRUE 1

int main (void) {
	int fifo;
	char c[100];

	if ((fifo = open("minhaFifo", OPENMODE)) < 0) { 
		puts ("Erro ao abrir a FIFO para escrita"); 
		return -1; 
	} 

	while (read(fifo, c, 100*sizeof(char)) > 0) {
		printf("Lido Fifo: %s\n",c);
	}
	close (fifo);
	return 0;
}
