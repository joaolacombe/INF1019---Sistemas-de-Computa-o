#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void handler (int sinal);

int main (void) {

	int n1,n2;
	void (*p)(int);
	
	p = signal(SIGFPE, handler); 
	printf("Endereco do manipulador anterior %p\n", p); 
	
	printf("Insira 2 numeros.\n");
	scanf("%d %d",&n1,&n2);

	printf("Soma %d\n",n1+n2);
	printf("Diferenca %d\n",n1-n2);
	printf("Multiplicacao: %d\n",n1*n2);
	printf("Divisao: %d\n",n1/n2);
	

	return 0;
}

void handler (int sinal) {
	printf("Divisão nao pode ser feita por 0!\n");
	exit(1);
}
