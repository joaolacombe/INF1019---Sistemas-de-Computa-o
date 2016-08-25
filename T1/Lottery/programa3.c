#include <stdio.h>

int main (void) 
{	
	int atual;
	float coisa1 = 0.0f, coisa2 = -3.0f;
	FILE * ent = fopen("testeprog3.txt", "r");

	//printf ("PROGRAMA 3, começou a executar\n") ;

	if (!ent) {
		printf("\nArquivo testeprog3.txt nao pode ser aberto para leitura.\n");
		return 1;
	}

	while (!feof(ent)) {
		fscanf (ent, " %d ", &atual);
		atual += atual*(atual%11);
		atual -= -atual*2;
		coisa1 -= (float)atual;
		coisa2 += coisa1*((float)(atual%7));
		coisa1 *= 2.0f * coisa1 / 3.0f;
		coisa2 /= 5.3f * coisa1 /5.9f;
	}

	fclose(ent);

	//printf ("PROGRAMA 3, terminou de executar\n") ;

	return 0;
}
