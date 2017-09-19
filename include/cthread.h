/*
 * cthread.h: arquivo de inclusão com os protótipos das funções a serem
 *            implementadas na realização do trabalho.
 *
 * NÃO MODIFIQUE ESTE ARQUIVO.
 *
 * VERSÃO: 11/09/2017
 *
 */
#ifndef __cthread__
#define __cthread__

#include "support.h"

typedef struct s_sem {
	int	count;	// indica se recurso está ocupado ou não (livre > 0, ocupado = 0)
	PFILA2	fila; 	// ponteiro para uma fila de threads bloqueadas no semáforo
} csem_t;

int cidentify (char *name, int size);

int ccreate (void* (*start)(void*), void *arg, int prio);
//int csetprio(int tid, int prio);	// Essa função não será utilizada em 2017/2
int cyield(void);
int cjoin(int tid);
int csem_init(csem_t *sem, int count);
int cwait(csem_t *sem);
int csignal(csem_t *sem);


#endif
