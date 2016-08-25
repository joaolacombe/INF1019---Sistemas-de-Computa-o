#include <stdio.h>

int main (void)
{
	int i, j;
	FILE* sai = fopen ("saidaprog5.txt", "w");
	
	if (!sai) {
		printf("\nArquivo saidaprog5.txt nao pode ser aberto para escrita.\n");
		return 1;
	}

	//printf ("PROGRAMA 5, começou a executar\n") ;
	
	for ( i = 0 ; i < 6000 ; i++) {
		for ( j = 0 ; j < 5000 ; j++)
			fputs ("Saida\t", sai);
		fputs ("SAIDA...\n", sai);
	}

	fclose (sai);
	
	//printf ("PROGRAMA 5, terminou de executar\n") ;

	return 0;
}
