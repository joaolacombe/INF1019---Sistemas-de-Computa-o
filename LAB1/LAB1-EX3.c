#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX 10

void imprime_vetor(int vet[],int n){
	int i;
	for(i=0;i<n;i++) {
		printf("%d ",vet[i]);
	}
	printf("\n");
}

int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}
int main () {

	int filho_pid;
	int vet[MAX],i;
	
	for(i=0;i<MAX;i++) {
		scanf("%d",&vet[i]);		
	}

	imprime_vetor(vet,10);
	filho_pid = fork();
	
	if (filho_pid != 0 ) { 
		int status;
		waitpid(filho_pid,&status,0);
		imprime_vetor(vet,10);
}
	else {
		qsort(vet,10,sizeof(int),cmpfunc);
		imprime_vetor(vet,10);
		exit(1);
}
	return 0;
}
