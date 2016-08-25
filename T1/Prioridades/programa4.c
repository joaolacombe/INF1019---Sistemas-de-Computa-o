#include <stdio.h>

int main (void) 
{	
	float atual;
	FILE * ent = fopen("testeprog4.txt", "r");
	FILE * sai = fopen("saidaprog4.txt", "w");

	//printf ("PROGRAMA 4, começou a executar\n") ;

	if (!ent) {
		printf("\nArquivo testeprog3.txt nao pode ser aberto para leitura.\n");
		return 1;
	}

	if (!sai) {
		printf("\nArquivo saidaprog4.txt nao pode ser aberto para escrita.\n");
		return 1;
	}

	while (!feof(ent)) {
		fscanf (ent, " %f ", &atual);
		atual *= -2.75f;
		fprintf(sai, "%f\n", atual);
	}

	fclose(ent);
	fclose(sai);

	//printf ("PROGRAMA 4, terminou de executar\n") ;

	return 0;
}
