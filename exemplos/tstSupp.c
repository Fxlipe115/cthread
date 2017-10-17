
/********************************************************************
	Support Library Test Program
	Vers. 17.1 - 25/04/2017
********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "support.h"

#define WINDOWS

int	vetor[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

int	Verifica(PFILA2 fila, int value);
int	ClearFila2(PFILA2 fila);
void	printGrade(int current);
void	printCurrentGrade(char *texto, int current);
void	printErrorMessage(char *texto);

#define	TOTAL_COUNT	18	// Numero total de testes

int main() {
	int	err;
	int	i,n;
	FILA2	fila;
	FILA2	fila2;
	int	correctCount;
	
	correctCount = 0;
	
	//-------------------------------------------------------------------------------
	// Informa a versao da biblioteca
	printf ("Support Version: %d/%d\n", Version()/2, 1+(Version()&1) );
	
	//-------------------------------------------------------------------------------
	// Informação de sizeof
	printf ("Medidas de sizeof(type)\n");
	printf ("\tsizeof int = %d (%d bits)\n", sizeof(int), 8*sizeof(int) );
	printf ("\tsizeof long int = %d (%d bits)\n", sizeof(long int), 8*sizeof(long int) );
	printf ("\tsizeof long long int = %d (%d bits)\n", sizeof(long long int), 8*sizeof(long long int) );
	
	//-------------------------------------------------------------------------------
	// Teste da ramdom
	printf ("(1)Teste de random\n");
	printf ("\tSequencia: %d - %d - %d\n", Random2(), Random2(), Random2() );
	
	//-------------------------------------------------------------------------------
	// Inicializa a fila
	printf ("(2) Teste de CreateFila2\n");
	if (CreateFila2(&fila)) {
		printErrorMessage ("Não conseguiu criar a fila.");
		printGrade(correctCount);
		exit(0);
	}
	correctCount++;
	printCurrentGrade("CreateFila2", correctCount);
	// fila = []
	
	//-------------------------------------------------------------------------------
	// Teste de fila vazia
	printf ("(3) Teste de FirstFila2\n");
	n = FirstFila2(&fila);
	if (n==0) {
		printErrorMessage ("FirstFila2 should return an error as queue is empty.\n");
		printGrade(correctCount);
		exit(0);
	}
	correctCount++;
	printCurrentGrade("FirstFila2 returned empty queue information.", correctCount);
	
	//-------------------------------------------------------------------------------
	// Teste de append (coloca vetor[0])
	printf ("(4) Teste de AppendFila2\n");
	if (AppendFila2(&fila, (void *)(vetor))) {
		printErrorMessage ("Could not APPEND item to the queue\n");
		printGrade(correctCount);
		exit(0);
	}
	correctCount++;
	printCurrentGrade("AppendFila2 worked.", correctCount);
	// fila = [0]

	//-------------------------------------------------------------------------------
	// Termina de preencher com mais 4 itens
	printf ("(5) Fill fila using AppendFila2\n");
	for (i=1; i<5; ++i) {
		if (AppendFila2(&fila, (void *)(vetor+i))) {
			printErrorMessage ("Could not append item to the queue\n");
			printGrade(correctCount);
			exit(0);
		}
	}
	correctCount++;
	printCurrentGrade("5 itens appended (AppendFila2).", correctCount);
	// fila = [0] [1] [2] [3] [4]
	
	//-------------------------------------------------------------------------------
	// Teste de First
	// Verifica se posicionou no inicio da fila
	// Verifica se pegou o elemento correto
	printf ("(6) Verify content at FirstFila2\n");
	if (FirstFila2(&fila)) {
		printErrorMessage ("FirstFila2 is NOT working\n");
		printGrade(correctCount);
		exit(0);
	}	
	if (Verifica(&fila, 0)) {
		printErrorMessage ("First queue content IS NOT correct\n");
		printGrade(correctCount);
		exit(0);
	}
	correctCount++;
	printCurrentGrade("GetAtIteratorFila2 got correct item.", correctCount);

	//-------------------------------------------------------------------------------
	// Teste de Last
	// Verifica se posicionou no final da fila
	printf ("(7) Verify content at LastFila2\n");
	if (LastFila2(&fila)) {
		printErrorMessage ("LastFila2 is NOT working\n");
		printGrade(correctCount);
		exit(0);
	}	
	if (Verifica(&fila, 4)) {
		printErrorMessage ("Last queue content IS NOT correct\n");
		printGrade(correctCount);
		exit(0);
	}
	correctCount++;
	printCurrentGrade("Working.", correctCount);
	
	//-------------------------------------------------------------------------------
	// Teste de Next
	printf ("(8) Verify NextFila2 operation\n");
	FirstFila2(&fila);
	for (n=i=0; i<5; ++i) {
		n += (*(int *)GetAtIteratorFila2(&fila));
		err = NextFila2(&fila);
		if (err!=0 && err!=(-NXTFILA_ENDQUEUE)) {
			printErrorMessage ("NextFila2 is NOT working (errcode=%d)\n");
			printGrade(correctCount);
			exit(0);
		}
	}
	if (err!=(-NXTFILA_ENDQUEUE)) {
		printErrorMessage ("NextFila2 does not returned END OF QUEUE\n");
		printGrade(correctCount);
		exit(0);
	}
	if (n!=10) { // Somatoriao dos conteudos colocados na fila
		printErrorMessage ("NextFila2 IS NOT pointing to next item ot the queue.\n");
		printGrade(correctCount);
		exit(0);
	}
	correctCount++;
	printCurrentGrade("Working.", correctCount);

	//-------------------------------------------------------------------------------
	// Teste de insersao AFTER no meio da lista
	printf ("(9) Verify InsertAfterIteratorFila2 operation, in the middle of the queue.\n");
	FirstFila2(&fila);
	NextFila2(&fila);
	NextFila2(&fila);	
	if (InsertAfterIteratorFila2(&fila, (void *)(vetor+9))) {
		printErrorMessage ("InsertAfterIteratorFila2 is NOT working\n");
		printGrade(correctCount);
		exit(0);
	}
	FirstFila2(&fila);
	NextFila2(&fila);
	NextFila2(&fila);	
	NextFila2(&fila);
	if (Verifica(&fila, 9)) {
		printErrorMessage ("Inserted node IS NOT at correct place\n");
		printGrade(correctCount);
		exit(0);
	}
	correctCount++;
	printCurrentGrade("Working.", correctCount);
	// fila = [0] [1] [2] [9] [3] [4]
	
	
	//-------------------------------------------------------------------------------
	// Teste de DELETE
	printf ("(10) Verify DeleteAtIteratorFila2 operation, in the middle of the queue.\n");
	FirstFila2(&fila);
	NextFila2(&fila);
	NextFila2(&fila);	
	NextFila2(&fila);	
	if (DeleteAtIteratorFila2(&fila)) {
		printErrorMessage ("DeleteAtIteratorFila2 is NOT working\n");
		printGrade(correctCount);
		exit(0);
	}
	if (Verifica(&fila, 3)) {
		printErrorMessage ("DeleteAtIteratorFila2 get away wrong node\n");
		printGrade(correctCount);
		exit(0);
	}
	correctCount++;
	printCurrentGrade("Working.", correctCount);
	// fila = [0] [1] [2] [3] [4]
	
	//-------------------------------------------------------------------------------
	// Teste de insersao BEFORE no meio da lista
	printf ("(11) Verify InsertBeforeIteratorFila2 operation (in the middle).\n");
	FirstFila2(&fila);
	NextFila2(&fila);
	NextFila2(&fila);	
	if (InsertBeforeIteratorFila2(&fila, (void *)(vetor+9))) {
		printErrorMessage ("InsertBeforeIteratorFila2 is NOT working\n");
		printGrade(correctCount);
		exit(0);
	}
	FirstFila2(&fila);
	NextFila2(&fila);
	NextFila2(&fila);	
	if (Verifica(&fila, 9)) {
		printErrorMessage ("inserted node IS NOT at correct place\n");
		printGrade(correctCount);
		exit(0);
	}
	correctCount++;
	printCurrentGrade("Working.", correctCount);
	// fila = [0] [1] [9] [2] [3] [4]
	FirstFila2(&fila);
	NextFila2(&fila);	
	NextFila2(&fila);	
	if (DeleteAtIteratorFila2(&fila)) {
		printErrorMessage ("DeleteAtIteratorFila2 is NOT working\n");
		printGrade(correctCount);
		exit(0);
	}
	// fila = [0] [1] [2] [3] [4]

	//-------------------------------------------------------------------------------
	// Teste de insert before no inicio da fila
	printf ("(12) Verify InsertBeforeIteratorFila2 operation, at beginning of the queue.\n");
	FirstFila2(&fila);
	if (InsertBeforeIteratorFila2(&fila, (void *)(vetor+9))) {
		printErrorMessage ("InsertBeforeIteratorFila2 is NOT working\n");
		printGrade(correctCount);
		exit(0);
	}
	FirstFila2(&fila);
	if (Verifica(&fila, 9)) {
		printErrorMessage ("Inserted node AT BEGINNING IS NOT at correct place\n");
		printGrade(correctCount);
		exit(0);
	}
	correctCount++;
	printCurrentGrade("Working.", correctCount);
	// fila = [9] [0] [1] [2] [3] [4]	
	
	//-------------------------------------------------------------------------------
	// delete inserted item
	printf ("(13) DeleteAtIteratorFila2 after insertion in the beginning\n");
	FirstFila2(&fila);
	NextFila2(&fila);
	NextFila2(&fila);	
	if (DeleteAtIteratorFila2(&fila)) {
		printErrorMessage ("DeleteAtIteratorFila2 is NOT working\n");
		printGrade(correctCount);
		exit(0);
	}
	// fila = [9] [0] [2] [3] [4]	
	if (Verifica(&fila, 2)) {
		printErrorMessage ("DeleteAtIteratorFila2 get away wrong node\n");
		printGrade(correctCount);
		exit(0);
	}
	correctCount++;
	printCurrentGrade("Working.", correctCount);
	
	//-------------------------------------------------------------------------------
	// Clear fila2 to verify empty queue
	printf ("(14) Clear fila\n");
	FirstFila2(&fila);
	for (i=0; i<10; ++i) {
		if (DeleteAtIteratorFila2(&fila))
			break;
	}
	// fila = []	
	if (AppendFila2(&fila, (void *)(&vetor[7]))) {
		printErrorMessage ("Could not append item after clear queue\n");
		printGrade(correctCount);
		exit(0);
	}
	// fila = [7]	
	FirstFila2(&fila);
	err = Verifica(&fila, 7);
	if (err) {
		switch(err) {
			case -1: printErrorMessage ("GetAtIteratorFila2 IS NOT working, after clear fila2\n"); break;
			case -2: printErrorMessage ("GetAtIteratorFila2 is getting a wrong item, after clear fila2\n"); break;
			default: printErrorMessage ("GetAtIteratorFila2 returned an error code\n"); break;
		}
		printGrade(correctCount);
		exit(0);
	}
	correctCount++;
	printCurrentGrade("Working.", correctCount);
	
	//-------------------------------------------------------------------------------
	// Interferencia entre duas filas
	printf ("(15) CreateFila2 a second queue\n");
	if (CreateFila2(&fila2)) {
		printErrorMessage ("Cound not create the second queue\n");
		printGrade(correctCount);
		exit(0);
	}
	correctCount++;
	printCurrentGrade("Working.", correctCount);
	
	//-------------------------------------------------------------------------------
	// Limpa as duas filas
	printf ("(16) Clear both queue\n");
	int	exitOnError = 0;
	if (ClearFila2(&fila)) {
		printErrorMessage ("Could not CLEAR first queue\n");
		exitOnError = 1;
	}	
	if (ClearFila2(&fila2)) {
		printErrorMessage ("Could not CLEAR an empty queue (the second queue)\n");
		exitOnError = 1;
	}
	if (exitOnError) {
		printGrade(correctCount);
		exit(0);
	}
	correctCount++;
	printCurrentGrade("Working.", correctCount);
	
	//-------------------------------------------------------------------------------
	// Preenche as duas filas com conteúdos diferentes
	printf ("(16) Fill both queue (different contents)\n");
	for (i=0; i<3; ++i) {
		if (AppendFila2(&fila, (void *)(vetor+i))) {
			printErrorMessage ("Could not append item to the first queue\n");
			exitOnError = 1;
			break;
		}
	}
	// fila = [0] [1] [2]
	for (i=5; i<8; ++i) {
		if (AppendFila2(&fila2, (void *)(vetor+i))) {
			printErrorMessage ("Could not append item to the second queue\n");
			exitOnError = 1;
			break;
		}
	}
	// fila = [0] [1] [2]
	// fila2 = [5] [6] [7]
	if (exitOnError) {
		printGrade(correctCount);
		exit(0);
	}
	correctCount++;
	printCurrentGrade("Working.", correctCount);

	//-------------------------------------------------------------------------------
	// Verifica o conteúdo da primeira fila
	printf ("(17) Verifies 3rd node of first queue\n");
	FirstFila2(&fila);
	NextFila2(&fila);
	NextFila2(&fila);
	if (Verifica(&fila, 2)) {
		printErrorMessage ("Third node content in the first queue IS NOT correct\n");
		printGrade(correctCount);
		exit(0);
	}
	correctCount++;
	printCurrentGrade("Working.", correctCount);
	
	//-------------------------------------------------------------------------------
	// Verifica o conteúdo da segunda fila
	printf ("(18) Verifies 3rd node of second queue\n");
	FirstFila2(&fila2);
	NextFila2(&fila2);
	NextFila2(&fila2);
	if (Verifica(&fila2, 7)) {
		printErrorMessage ("Third node content in the second queue IS NOT correct\n");
		printGrade(correctCount);
		exit(0);
	}
	correctCount++;
	printCurrentGrade("Working.", correctCount);
	
	//-------------------------------------------------------------------------------
	// Teste de acesso ao timer
	printf ("(19) timer functions -> manual verification\n");
	for (n=0; n<10; n++) {
		
		startTimer();
		for (i=0; i<100; i++);
		printf ("Delta=%u\n", stopTimer());
	}
	
	//-------------------------------------------------------------------------------
	printf ("SEM ERRO!\n");
	printGrade(correctCount);
	exit(0);
}


int	Verifica(PFILA2 fila, int value) {
	
	int *p;
	
	p = (int *)GetAtIteratorFila2(fila);
	if (p==NULL) {
		return -1;
	}
	if (*p!=value) {
		//printf ("VERIFICA %d==%d?", value, *p);
		return -2;
	}
	return 0;
}


int	ClearFila2(PFILA2 pFila) {
	
	FirstFila2(pFila);
	while (GetAtIteratorFila2(pFila)!=NULL) {
		if (DeleteAtIteratorFila2(pFila)) {
			return -1;
		}
	}
	return 0;
}

void	printGrade(int current) {
	printf ("GRADE IS (%d/%d): %d\n", current, TOTAL_COUNT, (100*current)/TOTAL_COUNT);
}

void	printCurrentGrade(char *texto, int current) {
	printf ("\tOK: %s (%d/%d): grade=%d\n", texto, current, TOTAL_COUNT, (100*current)/TOTAL_COUNT);
}

void	printErrorMessage(char *texto) {
	printf ("\tERROR: %s\n", texto);
}




