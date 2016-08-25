#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>


int main (void) { 

 	if (mkfifo("minhaFifo2", S_IRUSR | S_IWUSR) == 0) { 
		puts ("FIFO criada com sucesso"); 
	} 
	else {
		puts ("Erro na criação da FIFO"); 
		return -1; 
	}

	if(fork() == 0) {
		int fifo;
		char str[100] = "PRIMEIRO FILHO";
		if ((fifo = open("minhaFifo2", O_WRONLY)) < 0) { 
			puts ("Erro ao abrir a FIFO para escrita"); 
			return -1; 
		} 
		printf("Escrito primeira msg no FIFO\n");
		write(fifo, str, sizeof(str));
		close(fifo);
	}
	else {
		if(fork() == 0) {
			int fifo;
			char str[100] = "SEGUNDO FILHO";
			if ((fifo = open("minhaFifo2", O_WRONLY)) < 0) { 
				puts ("Erro ao abrir a FIFO para escrita"); 
				return -1; 
			} 

			printf("Escrito segunda msg no FIFO\n");
			write(fifo, str, sizeof(str));
			close(fifo);
		}
		else{
			int fifo;
			char str[100];

			if ((fifo = open("minhaFifo2", O_RDONLY)) < 0) { 
				puts ("Erro ao abrir a FIFO para escrita"); 
				return -1; 
			}
			waitpid(-1,NULL,0);
			waitpid(-1,NULL,0);


			while (read(fifo, str, 100*sizeof(char)) > 0 ){
				printf("Lido do Fifo: %s\n",str);
			}
			close(fifo);
		}
	}

	return 0;
} 
