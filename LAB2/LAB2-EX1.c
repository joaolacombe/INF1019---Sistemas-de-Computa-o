#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

void preenche_matriz(int *matriz,int nlinhas, int ncolunas) {
	int i,j;
	for (i=0; i < nlinhas; i++) {
		for (j=0; j < ncolunas; j++) {
			matriz[i*ncolunas+j] = rand()%11;
		}
	}
}
void imprime_matriz(int *matriz,int nlinhas, int ncolunas) {
	int i,j;
	for (i=0; i < nlinhas; i++) {
		for (j=0; j < ncolunas; j++) {
			printf("%d ",matriz[i*ncolunas+j]);
		}
		printf("\n");
	}
}

int main (int argc,char *argv[]) { 
	int seg1, seg2, seg3;
	int *m1, *m2, *m3;
	int id, status, nrows = 4, ncolumns = 4,i,qtdfilhos; 

	//seed para preencher matriz
	srand ( time(NULL) );
	
	seg1 = shmget (IPC_PRIVATE, sizeof (int)*nrows*ncolumns, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
	if (seg1 == -1) {
		puts ("Erro na criação de segmento de memoria compartilhada.");
		exit(1);
	}
	m1 = (int *) shmat (seg1, 0, 0);

	seg2 = shmget (IPC_PRIVATE, sizeof (int)*nrows*ncolumns, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
	if (seg2 == -1) {
		puts ("Erro na criação de segmento de memoria compartilhada.");
		exit(1);
	}
	m2 = (int *) shmat (seg2, 0, 0);

	seg3 = shmget (IPC_PRIVATE, sizeof (int)*nrows*ncolumns, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
	if (seg3 == -1) {
		puts ("Erro na criação de segmento de memoria compartilhada.");
		exit(1);
	}
	m3 = (int *) shmat (seg3, 0, 0);

	preenche_matriz(m1,nrows,ncolumns);
	preenche_matriz(m2,nrows,ncolumns);
	printf("Primeira matriz\n");
	imprime_matriz(m1,nrows,ncolumns);
	printf("Segunda matriz\n");
	imprime_matriz(m2,nrows,ncolumns);

	for (qtdfilhos = 0; qtdfilhos < nrows; qtdfilhos++) {
		id = fork();
		if (id < 0) { 
			puts ("Erro na criação do novo processo"); 
			exit (-2); 
		} 
		if (id != 0) {
			int j,index = qtdfilhos*ncolumns;
			for (j = 0; j < ncolumns; j++) {
				m3[index] = m1[index] + m2[index];
				index++;
			}
			break;
		}
	}
	if (id == 0) {
		for(i=0;i<qtdfilhos;i++){
			waitpid(-1,&status,0);
		}
		printf("Terceira matriz\n");
		imprime_matriz(m3,nrows,ncolumns);
	}

	shmdt (m1); 
	shmctl (seg1, IPC_RMID, 0); 
	shmdt (m2); 
	shmctl (seg2, IPC_RMID, 0); 
	shmdt (m3); 
	shmctl (seg3, IPC_RMID, 0);

	return 0; 
} 