
#include <stdio.h>
#include <string.h>
#include <ucontext.h>
#include "../include/support.h"
#include "../include/cthread.h"

int initializedCthreads = 0;

FILA2 blockedQueue;
FILA2 readyQueue;
FILA2 running;

ucontext_t scheduler;


void initializeCthreads(){
    int failed = 1;

    getcontext(&scheduler);
    ///inicializar esses parametros do scheduler
    /* ucontext_t *uc_link     pointer to the context that will be resumed
                        when this context returns
        sigset_t    uc_sigmask  the set of signals that are blocked when this
                        context is active
        stack_t     uc_stack    the stack used by this context
        mcontext_t  uc_mcontext a machine-specific representation of the saved
                        context
    */
    ///makecontext(&scheduler, ????????????????); //void makecontext(ucontext_t *, (void *)(), int, ...);
    /// running eh uma fila de um soh ou eh direto um TCB_t?

    failed = CreateFila2(&blockedQueue);
    if(failed){
        printf("Error: blocked queue initialization failed\n");
    }
    failed = CreateFila2(&readyQueue);
    if(failed){
        printf("Error: ready queue initialization failed\n");
    }
    failed = CreateFila2(&running);
    if(failed){
        printf("Error: running state initialization failed\n");
    }
}





int cidentify (char *name, int size){
    int printSize, strSize;
    char names[] = "Felipe de Almeida Graeff - 00261606\nJulia Eidelwein - 00274700\n";
    printSize = snprintf(name,size,"Group:\n%s",names);
    strSize = strlen(names) + strlen("Group:\n");
    if(printSize == strSize || printSize == size){
        return 0;
    }
    return -1;
}



