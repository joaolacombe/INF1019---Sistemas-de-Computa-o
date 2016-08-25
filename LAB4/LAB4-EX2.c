#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

void chamadaRecebida (int sinal);
void chamadaTerminada (int sinal);

static time_t ini_t;

int main (void) {

	void (*p)(int), (*p2)(int);
	
	p = signal(SIGUSR1, chamadaRecebida); 
	printf("Endereco do manipulador anterior SIGUSR1 %p\n", p); 
	p2 = signal(SIGUSR2, chamadaTerminada); 
	printf("Endereco do manipulador anterior SIGUSR2 %p\n", p2);

	while(1);

	return 0;
}

void chamadaRecebida (int sinal) {
	printf("Chamada recebida!\n");
	time(&ini_t);
}
void chamadaTerminada (int sinal) {
	time_t diff_t;
	time_t final_t;
	long int custoCentavos = 0;	

	time(&final_t);
	diff_t = difftime(final_t, ini_t);
	printf("Chamada terminada. Duracao: %li m %li s \n",diff_t/60, diff_t%60);
	
	if( diff_t <= 60) {
		custoCentavos = diff_t*2;
	}
	else {
		custoCentavos = 60*2;
		diff_t -= 60;
		custoCentavos += diff_t;	
	}
	printf("Custo ligacao: R$%li,%02li\n",custoCentavos/100,custoCentavos%100);
}
