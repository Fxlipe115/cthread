
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ucontext.h>
//#include "../include/support.h"
//#include "../include/cthread.h"
//#include "../include/cdata.h"
//#include "../include/insert.h"
#include "support.h"
#include "cthread.h"
#include "cdata.h"
#include "insert.h"


PFILA2 blockedQueue;
PFILA2 readyQueue;
PFILA2 cjoinQueue;
TCB_t *running;

TCB_t mainThread;

ucontext_t dispatcher;
char dispatcherStack[SIGSTKSZ];

int initializedCthreads = 0;
int tidTh = 1;


void unjoin(int tid){
    TCB_t *joinTh = 0, *blockTh = 0;
    if(FirstFila2(cjoinQueue) != 0){//error or empty queue
        return;
    }
    do{
        if(cjoinQueue->it == 0){
            break;
        } else {
            joinTh = (TCB_t *)GetAtIteratorFila2(cjoinQueue);
            if (joinTh->tid == tid){
                break;
            }
            joinTh = 0;
        }
    } while(NextFila2(cjoinQueue) == 0);

    if (joinTh){//found the thread to be taken of joinQueue

        if(FirstFila2(blockedQueue) != 0){//error or empty queue
            return;
        }
        do{
            if (blockedQueue->it == 0){
                break;
            }
            if(joinTh == blockTh){//found thread in blockedQueue, remove it from both queues and put it on readyQueue;
                DeleteAtIteratorFila2(blockedQueue);
                DeleteAtIteratorFila2(cjoinQueue);
                free(joinTh);
                blockTh->state = PROCST_APTO;
                AppendFila2(readyQueue, (void *)blockTh);
                break;
            }
        } while (NextFila2(blockedQueue) == 0);
    }
}


void* scheduler(){
    //TCB_t *mostPriorityTh;
    if(running){ // NULL = cyeld, !NULL = execution ended
        running->state = PROCST_TERMINO;
        unjoin(running->tid);
        free(running->context.uc_stack.ss_sp);
        free(running);
        running = 0;
    }
    if(FirstFila2(readyQueue) != 0){
        return 0;
    }

    //mostPriorityTh = (TCB_t *)GetAtIteratorFila2(readyQueue);
    running = (TCB_t *)GetAtIteratorFila2(readyQueue);
    DeleteAtIteratorFila2(readyQueue);
    running->state = PROCST_EXEC;
    setcontext(&running->context);
        ///continua...
    return 0;

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

    initializedCthreads = 1;
}

int ccreate (void* (*start)(void*), void *arg, int prio){
    TCB_t *newThread;
    if (!initializedCthreads){
        initializeCthreads();
    }
    newThread = malloc(sizeof(TCB_t));
    newThread->prio = 0;
    newThread->tid = tidTh;
    tidTh++;
    newThread->bTid = -1; //no blocking thread
    newThread->state = PROCST_APTO;

    getcontext(&newThread->context);
    newThread->context.uc_link = &dispatcher;
    newThread->context.uc_stack.ss_sp = malloc(SIGSTKSZ);
    newThread->context.uc_stack.ss_size = SIGSTKSZ;
    makecontext(&newThread->context, (void (*)(void))start, 1, arg);

    if(AppendFila2(readyQueue, (void*)newThread) != 0){
        return -1;
    }

    return newThread->tid;
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



