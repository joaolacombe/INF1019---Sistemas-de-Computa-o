/****************************************************************************************************
 *
 *	TRABALHO 1 - Sistemas de Computação
 *	Professor: Endler
 *	25/04/2016
 *
 *	Aluno:	João Pedro Garcia 1211768
 *	Aluno:	Thiago Klein      1321929
 *
 *	O programa lê, na ordem dada, as seguintes informações de um arquivo "entrada.txt" :
 *
 *		1. A palavra exec indicando mais um programa a ser executado ;
 *
 *		2. O nome do programa a ser executado, o nome do programa é uma string sempre de nome
 *       programaN, onde n é um numero inteiro de 1 a 7
 *
 *	Apos ler e armazenar as informações, o programa chama executa os processos na ordem cor-
 *	reta, conforme especificado no enunciado do trabalho.
 *
 ****************************************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/shm.h>


#define MAX_NUM_PROGRAMAS	7

/***************************************************************************************************/
//Estruturas de dados auxiliares.
struct infoProcesso{
    char nome[20]; /* Nome do programa referente ao processo */
    int pid;       /* PID do processo */
    int p;         /* Prioridade do processo (NULL caso o escalonador não seja por prioridades) */
    int status;    /* 0 caso rolando, 1 caso terminado */
}; typedef struct infoProcesso InfoProcesso ;

/***************************************************************************************************/

int interpretador(FILE * fp1, FILE * fp2, InfoProcesso vetorInfo[], int * numProgramas, int * statusInterp);
int criaProcesso(int * pidProcesso, int posicao, char * nomeProcesso) ;
int escalonador(FILE * fp2, InfoProcesso vetorInfo[], int * numProgramas, int * statusInterp) ;
int criaPrograma(int n) ;

/***************************************************************************************************/

int main (void)
{
    InfoProcesso * vetorInfo ;
    int * numProgramas;
    int * statusInterp;
    int pidInterpretador;
    int retorno ;
    int i;
    
    int segmentoInfo, segmentoNum, segmentoTerminado;
    
    FILE * fp1 = fopen ("exec.txt", "r") ; /* Abertura de arquivo de entrada */
    FILE * fp2 = fopen ("saida.txt", "w") ;   /* Abertura de arquivo de saida   */
    
    segmentoInfo = shmget (IPC_PRIVATE, sizeof (InfoProcesso) * MAX_NUM_PROGRAMAS, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    vetorInfo = (InfoProcesso*)shmat(segmentoInfo,0,0);
    
    segmentoNum = shmget (IPC_PRIVATE, sizeof (int), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    numProgramas = (int*)shmat(segmentoNum,0,0);
    
    segmentoTerminado = shmget (IPC_PRIVATE, sizeof (int), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    statusInterp = (int*)shmat(segmentoTerminado,0,0);
    
    *numProgramas = 0;
    *statusInterp = 0;
    
    for (i = 0; i < MAX_NUM_PROGRAMAS; i++){
        vetorInfo[i].status = 1;
        vetorInfo[i].p = 1000;
    }
    
    if (fp1 == NULL || fp2 == NULL)           /* Teste de validação de abertura */
    {
        fprintf (fp2, "Erro na abertura de arquivo...") ;
        return 1 ;
    }
    
    pidInterpretador = fork();
    
    if(pidInterpretador == 0){    // Filho - Interpretador
        
        retorno = interpretador (fp1, fp2, vetorInfo, numProgramas, statusInterp) ;
        
        if (retorno != 0)
        {
            fprintf (fp2, "Erro no interpretador\n") ;
            return 1 ;
        }
        
    }
    
    else {              // Pai - Escalonador
        while(*numProgramas == 0); //Espera ler ao menos um programa para iniciar escalonamento
        
        escalonador(fp2, vetorInfo, numProgramas, statusInterp) ; //Chama funcao do escalonador
        printf("Escalonamento chegou ao fim!\n");
        kill(pidInterpretador, SIGKILL);
    }
    
    fclose (fp1) ;
    fclose (fp2) ;
    shmdt(statusInterp);
    shmdt(numProgramas);
    shmdt(vetorInfo);
    
    
    return 0 ;
    
    
}



/****************************************************************************************************
 *
 *	Função: interpretador
 *
 ****************************************************************************************************/

int interpretador (	FILE * fp1,	FILE * fp2, InfoProcesso vetorInfo[], int * numProgramas, int * statusInterp)
{
    int	ret, i; /* ret armazena retorno da função escalonador	*/
    char exec[12];            /* auxiliar na leitura do arquivo lendo "exec" ou "prioridade="*/
    
    *statusInterp = 0;        /* Marca que comecou a interpretar */
    *numProgramas = 0;
    i = 0;                    /* Zera as variaveis antes de entrar no While*/
    
    while(fscanf (fp1, "%s", exec) == 1){ /* inicia a leitura do arquivo entrada */
        if(i != 0){
            sleep(3);
        }
        
        fscanf(fp1,"%s", vetorInfo[i].nome);
        (*numProgramas)++;
        i++;
        
    }
    
    *statusInterp = 1; //Marca que terminou de interpretar
    while(0);
    
    return 0 ;
}



/****************************************************************************************************
 *
 *	Função: criaProcesso
 *
 ****************************************************************************************************/

int criaProcesso (int * pidProcesso, int posicao, char * nomeProcesso)
{
    int pid ;
    
    if(!strcmp(nomeProcesso, "programa1"))
        pid = criaPrograma(1) ;
    
    else if(!strcmp(nomeProcesso, "programa2"))
        pid = criaPrograma(2) ;
    
    else if(!strcmp(nomeProcesso, "programa3"))
        pid = criaPrograma(3) ;
    
    else if(!strcmp(nomeProcesso, "programa4"))
        pid = criaPrograma(4) ;
    
    else if(!strcmp(nomeProcesso, "programa5"))
        pid = criaPrograma(5) ;
    
    else if(!strcmp (nomeProcesso, "programa6"))
        pid = criaPrograma(6) ;
    
    else if(!strcmp (nomeProcesso, "programa7" ))
        pid = criaPrograma(7) ;
    
    else
    {
        printf("Nome do programa invalido. Não foi possivel criar o processo");
        return -1 ;
    }
    
    if ( pid == -1 )
    {
        printf("PID -1. Não foi possivel criar o processo");
        return -1 ;
    }
    
    *pidProcesso = pid ;
    return 0 ;
}



/****************************************************************************************************
 *
 *	Funções: criaPrograma
 *
 *	Retorno:
 *		pid - pid do processo criado
 *		0 - quando não consegue executar o fork
 *
 ****************************************************************************************************/

int criaPrograma (int n)
{
    int pid ;
    char nomePrograma[10] = "programa";
    sprintf(nomePrograma, "programa%d", n);
    
    pid = fork ()	; /* Executa o fork */
    
    if ( pid < 0 ){ /* Com problemas no fork retorna erro */
        return 0 ;
    }
    
    else if ( pid != 0 ){ /* PAI */
        kill ( pid, SIGSTOP ) ;	/* Pausa processo filho no início da execução	*/
        return pid ; 			/* Retorna o pid do filho					*/
    }
    
    else{ /* FILHO */
        execl(nomePrograma, "nada", NULL) ;
    }
    
    return 0 ;
}


/****************************************************************************************************
 *
 *	Função: escalonador
 *
 ****************************************************************************************************/

int escalonador (FILE * fp2, InfoProcesso vetorInfo[], int * numProgramas, int * statusInterp)
{
    int i, j, k, temp, status, pid, ultimoNumero, ret, terminados ;
    int tempo = 3;
    int firstTime = 0;
    int lastTime = time(NULL);
    
    fprintf (fp2, "Escalonamento escolhido: ROUND ROBIN\n") ;
    
    i = 0;
    j = 0;
    k = 0;
    terminados = 0;
    ultimoNumero = 0;
    
    while(*statusInterp == 0 || terminados < *numProgramas){
        pid_t result;
        tempo = lastTime - firstTime;
        if(tempo >= 3){
            if(ultimoNumero < *numProgramas){
                ret = criaProcesso(&(vetorInfo[*numProgramas -1].pid), *numProgramas -1, vetorInfo[*numProgramas -1].nome);
                vetorInfo[*numProgramas -1].status = 0;
                ultimoNumero = *numProgramas;
                
                if ( ret == -1 ){
                    fprintf (fp2, "Não foi possivel criar o processo ou o programa %s nao existe...\n", vetorInfo[i].nome) ;
                    return -1 ;
                }
                fprintf (fp2, "\n -----------\nEntrada de novo processo na fila\nNome: %s\nPid: %d\n---------------\n\n", vetorInfo[*numProgramas-1].nome, vetorInfo[*numProgramas-1].pid);
                printf("\n -----------\nEntrada de novo processo na fila\nNome: %s\nPid: %d\n---------------\n\n", vetorInfo[*numProgramas-1].nome, vetorInfo[*numProgramas-1].pid);
                
            }
            tempo = 0;
            firstTime = time(NULL);
        }
        
        if (vetorInfo[i].status == 0){   //Caso o programa nao tenha terminado
            printf ("Em execucao - Programa %s - Pid %d\n", vetorInfo[i].nome, vetorInfo[i].pid) ;
            fprintf (fp2, "Em execucao - Programa %s - Pid %d\n", vetorInfo[i].nome, vetorInfo[i].pid) ;
            kill(vetorInfo[i].pid, SIGCONT) ;        //Roda programa
            usleep(500000);
        
            result = waitpid(vetorInfo[i].pid, &status, WNOHANG);
            
            if (result == 0) {                                          //Nao terminou, entao pausa.
                printf ("Pausando - Programa %s\n\n", vetorInfo[i].nome) ;
                fprintf (fp2 ,"Pausando - Programa %s\n\n", vetorInfo[i].nome) ;
                kill (vetorInfo[i].pid, SIGSTOP) ;
            } else if (result == -1) {                                  //Terminou com erro
                return -1;
            } else {                                                    //Terimou certo
                printf ("Programa %s terminou sua execução...\n\n", vetorInfo[i].nome) ;
                fprintf (fp2 ,"Programa %s terminou sua execução...\n\n", vetorInfo[i].nome) ;
                vetorInfo[i].status = 1;                 //Marca como finalizado
                terminados++;
            }
        }
        lastTime = time(NULL);
        i++;
        if(i == *numProgramas){
            i = 0;
        }
    }
    /* Final caso LISTA DE ROUND ROBIN */
    return 0;
}
