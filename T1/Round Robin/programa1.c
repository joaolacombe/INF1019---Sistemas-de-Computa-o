#include <stdio.h>

int main (void)
{
	int i, j;
	double k = 0.0;	

	//printf ("PROGRAMA 1, começou a executar\n") ;
	
	for ( i = 0 ; i < 5000 ; i++)
		for ( j = 0 ; j < 50000 ; j++)
			k += (double) (j - i);

	//printf ("PROGRAMA 1, terminou de executar\n") ;

	return 0;
}
