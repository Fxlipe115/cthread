
#include <stdio.h>
#include <string.h>
#include <ucontext.h>
#include "../include/support.h"
#include "../include/cthread.h"
#include "../include/cdata.h"
#include "../include/insert.h"


PFILA2 blockedQueue;
PFILA2 readyQueue;
PFILA2 cjoinQueue;
TCB_t *running;

TCB_t mainThread;

ucontext_t dispatcher;
char dispatcherStack[SIGSTKSZ];

int initializedCthreads = 0;
int tid = 1;


void unlockJoin(int tid){
    ///continua...
}


void scheduler(){
    if(running){ // NULL = cyeld, !NULL = execution ended
        running->state = PROCST_TERMINO;
        unlockJoin(running->tid);
        ///continua...
    }
}


void initializeCthreads(){
    int failed = 1;

    failed = CreateFila2(blockedQueue);
    if(failed){
        printf("Error: blocked queue initialization failed\n");
    }
    failed = CreateFila2(readyQueue);
    if(failed){
        printf("Error: ready queue initialization failed\n");
    }
    failed = CreateFila2(cjoinQueue);
    if(failed){
        printf("Error: join queue initialization failed\n");
    }

    //iterator context
    getcontext(&dispatcher);
    dispatcher.uc_link = 0;
    dispatcher.uc_stack.ss_sp = dispatcherStack;
    dispatcher.uc_stack.ss_size = SIGSTKSZ;//constant SIGSTKSZ is commonly used
    ///makecontext(&dispatcher, ????????????????); //void makecontext(ucontext_t *, (void *)(), int, ...);

    //get main thread context
    mainThread.tid = 0; // main tid is 0
    mainThread.prio = 0; //threads are created with highest priority
    mainThread.state = PROCST_EXEC;
    getcontext(&mainThread.context);

    running = &mainThread;

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



