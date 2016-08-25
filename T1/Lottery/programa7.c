#include <stdio.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int main (void)
{
	int i;
    float atual;
    double num;
    FILE * ent = fopen("testeprog7.txt", "r");
    FILE * sai = fopen("saidaprog7.txt", "w");
	
    if (!ent) {
        printf("\nArquivo testeprog7.txt nao pode ser aberto para leitura.\n");
        return 1;
    }
    
	if (!sai) {
		printf("\nArquivo saidaprog7.txt nao pode ser aberto para escrita.\n");
		return 1;
	}

	//printf ("PROGRAMA 7, começou a executar\n") ;
    num = 1;
	while(fscanf (ent, "%f ", &atual) == 1) {
        for ( i = 0 ; i < 5000000 ; i++){
            num += (double) atual;
			fprintf(sai, "%.2lf ", num);
        }
		fputs ("SAIU DE UM FOR...\n\n", sai);
	}

	fclose (sai);
	
	//printf ("PROGRAMA 7, terminou de executar\n") ;

	return 0;
}
