#include <stdio.h>

int main (void)
{
	int i, j;
    float atual;
    FILE * ent = fopen("testeprog6.txt", "r");
    FILE * sai = fopen("saidaprog6.txt", "w");
	
    if (!ent) {
        printf("\nArquivo testeprog6.txt nao pode ser aberto para leitura.\n");
        return 1;
    }
    
	if (!sai) {
		printf("\nArquivo saidaprog6.txt nao pode ser aberto para escrita.\n");
		return 1;
	}

	//printf ("PROGRAMA 6, começou a executar\n") ;
	
	while (fscanf (ent, " %f ", &atual) == 1) {
        for(i = 0; i < 5000; i++){
            for ( j = 0 ; j < 500 ; j++)
                fprintf(sai, "%.2f ", atual);
            fputs ("SAIU DE UM FOR...\n\n", sai);
        }
	}

	fclose (sai);
	
	//printf ("PROGRAMA 6, terminou de executar\n") ;

	return 0;
}
