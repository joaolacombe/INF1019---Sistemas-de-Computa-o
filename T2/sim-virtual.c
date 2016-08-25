/****************************************************************************************************
 *
 *	TRABALHO 2 - Sistemas de Computação
 *	Professor: Markus Endler
 *	03/06/2016
 *
 *	Aluno:	João Pedro Garcia 1211768
 *	Aluno:	Thiago Klein      1321929
 *
 *	Simulador Memoria Virtual
 *
 * O simulador deverá ter os seguintes quatro argumentos de linha de commando:	  	  
 *  a) o algoritmo de  susbtituição de página sendo simulado (LRU/NRU/SEG)	  
 *  b) o arquivo contendo a	sequência de endereços de memoria acessados	(arq.log)	  
 *  c) o tamanho de cada página/quadro de página em KB (tamanhos a serem suportados 8 a 32 KB)  
 *  d) o tamanho total de memoria física disponível em KB (faixa de valores: 128 KB a 16 MB).	
 *  e) (opcional) Nivel detalhamento para modo DEBUG: -X
 *			X - indica nivel de detalhamento, 1,2,3 sendo 3 modo interativo. 
 *
 * Exemplos:
 *		sim-virtual LRU simulador.log 16 128 
 *		sim-virtual NRU matriz.log 32 256 -1
 *
 * 
 * Cada	linha do arquivo (.log) conterá um endereço de memória acessado	(em representação hexa-­‐decimal),
 * seguido das letras R ou W, indicando um	acesso de leitura ou escrita, respectivamente.
 *
 * Exemplo:
 * 		0700ff10 R
 * 		2f6965a0 W
 *
 ****************************************************************************************************/



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#define true 1
#define false 0


/*
 * Tipos
*/
typedef char bool;
typedef struct page Page;
typedef struct no No;
typedef struct lista Lista;

struct page {
	bool r; //FLAG pagina referenciada
	bool m; //FLAG pagina modificada
	bool present;
	int pageIndex;
	int ultimoAcesso;
};
/*
 * Funcoes
*/

void t_error(char* msg);
int t_lru(Page **memVirtual, int *memFisica, int qtdQuadros, int *pageWrite);
int t_nru(Page **memVirtual, int *memFisica, int qtdQuadros, int *pageWrite, int timer);
int t_seg(Page **memVirtual, int *memFisica, int qtdQuadros, int *pageWrite, int timer);


/*
 *  
 *	IMPLEMENTACAO
 * 
*/

int main(int argc, char* argv[]) {

	unsigned addr;
	char rw;
	FILE *input;
	char *algoritmo,*pathInput;
	int i,tamPagina = 0, tamMem = 0, pageFault = 0, pageWrite = 0, qtdQuadros, aux, bitsIndice, maximoPaginas, timer, quadrosUsados = 0;
	int DEBUG = 0;
	Page **memVirtual;
	int *memFisica;
	FILE* output;

	//REDIRECIONANDO SAIDA PARA ARQUIVO SAIDA.TXT
	output = fopen("saida.txt","w");
	if (output == NULL)
		t_error("Erro abrir saida.txt");

	//dup2(fileno(output),1);

	/* Leitura de Parametros */
	if (argc < 5 || argc > 6) {
		t_error("Argumentos Insuficientes.");
	}

	algoritmo = argv[1];
	if ( strcmp(algoritmo,"LRU") != 0 && strcmp(algoritmo,"NRU") != 0 && strcmp(algoritmo,"SEG") != 0) {
		t_error("Algoritmo de substituicao invalido (LRU/NRU/SEG).");
	}

	pathInput = argv[2];
	input = fopen(pathInput,"r");
	if(input == NULL)
		t_error("Erro ao abrir arquivo entrada.");

	sscanf(argv[3],"%d",&tamPagina);
	if(tamPagina < 8 || tamPagina > 32)
		t_error("Tamanho de pagina nao suportado. Apenas 8-32 KB");

	sscanf(argv[4],"%d",&tamMem);
	if(tamMem < 126 || tamMem > 16384)
		t_error("Tamanho de memoria fisica invalido. Apenas 128-16384KB (16MB)");

	if (argc == 6) {
		sscanf(argv[5],"-%d",&DEBUG);
		if (DEBUG < 1 || DEBUG > 3)
			t_error("Nivel detalhamento modo debug invalido. Apenas 1-3");
		printf("Modo Debug Ativo Nivel Detalhamento %d.\n", DEBUG );
	}

	/**************************/
	if (DEBUG)
		printf("Executando o Simulador...\n");

	/* Quantidade de quadros na memoria fisica */
	qtdQuadros = tamMem/tamPagina;

	/* Pegando a quantidade de bits relativos ao indice da pagina */
	aux = tamPagina*1024;
	bitsIndice = 0;
	while(aux > 1) {
		bitsIndice++;
		aux /= 2;
	}

	/* Pegando tamanho maximo da tabela de paginas */
	aux = 32 - bitsIndice;
	maximoPaginas = 2;
	while (aux) {
		maximoPaginas *= 2;
		aux--;
	}

	memVirtual = (Page**) malloc(maximoPaginas*sizeof(Page*));
	if(memVirtual == NULL)
		t_error("Memoria Insuficiente para memoria virtual.");

	for(i=0; i < maximoPaginas; i++) {
		memVirtual[i] = (Page*) malloc(sizeof(Page));
		memVirtual[i]->r = false;
		memVirtual[i]->m = false;
		memVirtual[i]->ultimoAcesso = -1;
		memVirtual[i]->present = false;
		memVirtual[i]->pageIndex = 0;
	}

	memFisica = (int*) malloc (qtdQuadros*sizeof(int));
	if (memFisica == NULL)
		t_error("Memoria Insuficiente para memoria virtual.");


	timer = 0;
	quadrosUsados = 0;
	while (fscanf(input, "%x %c ", &addr, &rw) != EOF) {
		
		unsigned pageIndex;
		Page *p;
		
		pageIndex = addr >> bitsIndice;
	 	p = memVirtual[pageIndex];

	 	if(DEBUG) {
	 		printf("Tempo: %d Pagina %d Acesso %c\n",timer,pageIndex,rw);
	 	}

		if(p->present) {
			 if(DEBUG >=2)
			 	printf("Pagina presente em quadro: %d\n",p->pageIndex);
		}
		else { //PAGINA NAO PRESENTE EM MEMORIA
			int index;
			pageFault++;
			//Ainda existem quadros disponiveis na memoria
			if( quadrosUsados < qtdQuadros) {
				 if(DEBUG >=2)
				 	printf("Quadro %d vazio. Pagina movida\n",quadrosUsados);
				index = quadrosUsados;
				quadrosUsados++;			
			}
			else {	//Sem espaço, chama algoritmo e substituicao
				if (strcmp(algoritmo,"NRU") == 0)
					index = t_nru(memVirtual, memFisica, qtdQuadros, &pageWrite,timer);
				else if (strcmp(algoritmo,"LRU") == 0)
					index = t_lru(memVirtual, memFisica, qtdQuadros, &pageWrite);
				else
					index = t_seg(memVirtual, memFisica, qtdQuadros, &pageWrite,timer);
				if(DEBUG >=2)
			 		printf("Sem moldura. Alg Subst removeu pagina: %d para dar espaco.\n",index);
			}
				p->present = true;
				p->pageIndex = index;
				memFisica[index] = pageIndex;

		}

		p->r = true;
		p->present = true;

		if (rw == 'W')
			p->m = true;
		p->ultimoAcesso = timer;


		if (DEBUG) {
			int k;
			for (k = 0; k < qtdQuadros; k++){
				if (k < quadrosUsados) {
					Page *dpage = memVirtual[memFisica[k]];
					printf("Quadro %d: %d R:%d M:%d ",k,memFisica[dpage->pageIndex],dpage->r,dpage->m);
					if (DEBUG >= 2)
						printf("Time ultimo Acesso:%d /",dpage->ultimoAcesso);
					else
						printf("/");
				}
				else
					printf("X /");
			}
			printf("\n\n");
			if (DEBUG > 2) {
				sleep(2);
			}
		}

		timer++;		
	}
	

	printf("Arquivo de Entrada: %s\n",pathInput );
	printf("Tamanho da Memoria fisica: %d KB\n",tamMem );
	printf("Tamanho das Paginas: %d KB\n",tamPagina );
	printf("Algoritmo de Substituicao: %s\n", algoritmo);
	printf("Numero de Faltas de Paginas: %d\n",pageFault);
	printf("Numero de Paginas Escritas: %d\n",pageWrite);

	free(memVirtual);
	fclose(input);
	fclose(output);

	return 0;	
}

void t_error(char* msg) {
	fprintf(stderr, "%s\n", msg);
	exit(1);
}

int t_lru(Page **memVirtual, int *memFisica, int qtdQuadros, int *pageWrite) {

	int i, last = 0;
	Page *lastPage;

	for ( i = 1; i < qtdQuadros; i ++) {
		Page *p = memVirtual[memFisica[i]];
		if (p->ultimoAcesso < memVirtual[memFisica[last]]->ultimoAcesso)
			last = i;
	}
	lastPage = memVirtual[memFisica[last]];
	lastPage->present = false;
	lastPage->r = false;
	if (lastPage->m == true) {
		(*pageWrite)++;
		lastPage->m = false;
	}
	lastPage->pageIndex = 0;
	lastPage->ultimoAcesso = 0;
	return last;
}	

int get_categoria(Page *p) {

	if (p->r == false && p->m == false)
		return 1;
	if (p->r == false && p->m == true)
		return 2;
	if (p->r == true && p->m == false)
		return 3;
	else
		return 4;
}

int t_nru(Page **memVirtual, int *memFisica, int qtdQuadros, int *pageWrite, int timer) {

	int i, last = 0, lastCat = 5;
	Page *lastPage;

	for ( i = 0; i < qtdQuadros; i ++) {
		Page *p = memVirtual[memFisica[i]];
		int cat;

		if (p->ultimoAcesso < timer - 20) {
			p->r = false;
		}

		cat = get_categoria(p);
		if (cat < lastCat) {
			last = i;
			lastCat = cat;
			if (lastCat == 1)
				break;
		}
	}
	lastPage = memVirtual[memFisica[last]];
	lastPage->present = false;
	lastPage->r = false;
	if (lastPage->m == true) {
		(*pageWrite)++;
		lastPage->m = false;
	}
	lastPage->pageIndex = 0;
	lastPage->ultimoAcesso = 0;
	return last;
	}

int t_seg(Page **memVirtual, int *memFisica, int qtdQuadros, int *pageWrite, int timer) {

	static int head = 0;
	int i, last = 0, aux;
	Page *lastPage;
	aux = head;

	for ( i = 0; i < qtdQuadros; i ++) {
		Page *p = memVirtual[memFisica[aux%qtdQuadros]];

		if(p->ultimoAcesso < timer - 20)
			p->r = 0;

		if (p->r == true) {
			head++;
			p->r = false;
		}
		else {
			last = aux%qtdQuadros;
			head++;
			break;
		}
		aux++;
	}
	if (head == qtdQuadros)
		head = 0;

	lastPage = memVirtual[memFisica[last]];
	lastPage->present = false;
	lastPage->r = false;
	if (lastPage->m == true) {
		(*pageWrite)++;
		lastPage->m = false;
	}
	lastPage->pageIndex = 0;
	lastPage->ultimoAcesso = 0;
	return last;
}
