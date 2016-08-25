#include <stdio.h>

int main (void)
{
	int i, j;
	double k = 1.0;
	
	//printf ("PROGRAMA 2, começou a executar\n") ;
	
	for ( i = 0 ; i < 5000 ; i++)
		for ( j = 0 ; j < 5000 ; j++)
			k *= (double) (j + i + 1);

	//printf ("PROGRAMA 2, terminou de executar\n") ;

	return 0;
}
