
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


FILA2 blockedQueue;
FILA2 readyQueue;
FILA2 cjoinQueue;
TCB_t *running;

TCB_t mainThread;

ucontext_t dispatcher;
char dispatcherStack[SIGSTKSZ];

int initializedCthreads = 0;
int tidTh = 1;


void unjoin(int tid){
    TCB_t *joinTh = 0, *blockTh = 0;
    if(FirstFila2(&cjoinQueue) != 0){//error or empty queue
        return;
    }
    do{
        if(cjoinQueue.it == 0){
            break;
        } else {
            joinTh = (TCB_t *)GetAtIteratorFila2(&cjoinQueue);
            if (joinTh->bTid == tid){
                break;
            }
            joinTh = 0;
        }
    } while(NextFila2(&cjoinQueue) == 0);

    if (joinTh){//found the thread to be taken off of joinQueue

        if(FirstFila2(&blockedQueue) != 0){//error or empty queue
            return;
        }
        do{
            if (blockedQueue.it == 0){
                break;
            }
            if(joinTh == blockTh){//found thread in blockedQueue, remove it from blocked queue and put it on readyQueue;
                DeleteAtIteratorFila2(&blockedQueue);
                DeleteAtIteratorFila2(&cjoinQueue);
                free(joinTh);
                blockTh->state = PROCST_APTO;
                AppendFila2(&readyQueue, (void *)blockTh);
                break;
            }
        } while (NextFila2(&blockedQueue) == 0);
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
    if(FirstFila2(&readyQueue) != 0){
        return 0;
    }

    //mostPriorityTh = (TCB_t *)GetAtIteratorFila2(readyQueue);
    running = (TCB_t *)GetAtIteratorFila2(&readyQueue);
    DeleteAtIteratorFila2(&readyQueue);
    running->state = PROCST_EXEC;
    setcontext(&running->context);
        ///continua...
    return 0;

}


void initializeCthreads(){
    int failed = 1;
    failed = CreateFila2(&blockedQueue);
    if(failed){
        printf("Error: blocked queue initialization failed\n");
    }
    failed = CreateFila2(&readyQueue);
    if(failed){
        printf("Error: ready queue initialization failed\n");
    }
    failed = CreateFila2(&cjoinQueue);
    if(failed){
        printf("Error: join queue initialization failed\n");
    }

    //iterator context
    getcontext(&dispatcher);
    dispatcher.uc_link = 0;
    dispatcher.uc_stack.ss_sp = dispatcherStack;
    dispatcher.uc_stack.ss_size = SIGSTKSZ;//constant SIGSTKSZ is commonly used
    makecontext(&dispatcher, (void(*)(void))scheduler, 0);

    //saving main thread context
    mainThread.tid = 0; // main tid is 0
    mainThread.prio = 0; //threads are created with highest priority
    mainThread.state = PROCST_EXEC;
    getcontext(&mainThread.context);

    running = &mainThread;

    startTimer();

    initializedCthreads = 1;
}

int ccreate (void* (*start)(void*), void *arg, int prio){
    TCB_t *newThread;
    if (!initializedCthreads){
        initializeCthreads();
    }
    newThread = (TCB_t*)malloc(sizeof(TCB_t));
    newThread->prio = prio;
    newThread->tid = tidTh;
    tidTh++;
    newThread->bTid = -1; //no blocking thread
    newThread->state = PROCST_APTO;

    getcontext(&newThread->context);
    newThread->context.uc_link = &dispatcher;
    newThread->context.uc_stack.ss_sp = (char*)malloc(SIGSTKSZ);
    newThread->context.uc_stack.ss_size = SIGSTKSZ;
    makecontext(&newThread->context, (void (*)(void))start, 1, arg);

    if(AppendFila2(&readyQueue, (void*)newThread) != 0){
    ///if(InsertByPrio(&readyQueue, (void*)newThread) != 0){
        return -1;
    }

    return newThread->tid;
}


int cyield(void){
    TCB_t *benevolentTh;
    unsigned int execTime;
    if(!initializedCthreads){
        initializeCthreads();
    }
    if(FirstFila2(&readyQueue) != 0){
        return 0;
    }
    benevolentTh = running;
    benevolentTh->state = PROCST_APTO;
    execTime = stopTimer();
    benevolentTh->prio = benevolentTh->prio + execTime;
    //benevolentTh->prio = benevolentTh->prio + stopTimer();
    ///printf("prio: %d time: %u\n", benevolentTh->prio, execTime);
    if(AppendFila2(&readyQueue, (void*)benevolentTh) != 0){
    ///if(InsertByPrio(&readyQueue, (void*)benevolentTh) != 0){
        startTimer();
        return -1;
    }
    running = 0;
    startTimer();
    swapcontext(&benevolentTh->context, &dispatcher);
    return 0;
}

int isOnQueue(PFILA2 queue, int tid){
    TCB_t *thread;
    if(FirstFila2(queue) != 0){
        return 0;
    }
    do{
        if(queue->it == 0){
            break;
        }
        thread = (TCB_t*)GetAtIteratorFila2(queue);
        if(thread->tid == tid){
            return 1;
        }
    }while(NextFila2(queue) == 0);
    return 0;
}

int alreadyJoined(PFILA2 queue, int tid){
    TCB_t *thread;
    if(FirstFila2(queue) != 0){
        return 0;
    }
    do{
        if(queue->it == 0){
            break;
        }
        thread = (TCB_t*)GetAtIteratorFila2(queue);
        if(thread->bTid == tid){
            return 1;
        }
    }while(NextFila2(queue) == 0);
    return 0;
}

int cjoin(int tid){
    TCB_t *thread, *joinTh;
    unsigned int execTime;
    if(!initializedCthreads){
        initializeCthreads();
    }
    if(tid == 0){
        //printf("ERROR: you cannot make a cjoin for main!\n");
        return -1;
    }
    if(alreadyJoined(&cjoinQueue, tid)){
        //printf("ERROR: you can only make a cjoin for a thread once at a time!\n");
        return -1;
    }
    if(isOnQueue(&readyQueue, tid) || isOnQueue(&blockedQueue, tid)){
        thread = running;
        joinTh = thread;
        joinTh->bTid = tid;
        if(AppendFila2(&cjoinQueue, (void*)joinTh) != 0){
        //if(InsertByPrio(&cjoinQueue, (void*)joinTh) != 0){
            printf("Error: failed inserting in join queue");
        }
        thread->state = PROCST_BLOQ;
        execTime = stopTimer();
        thread->prio = thread->prio + execTime;
        ///printf("prio: %d time: %u\n", thread->prio, execTime);
        //thread->prio = thread->prio + stopTimer();
        if(AppendFila2(&blockedQueue, (void*)thread) != 0){
        ///if(InsertByPrio(&blockedQueue, (void*)thread) != 0){
            printf("Error: failed inserting in blocked queue");
        }
        running = 0;
        startTimer();
        swapcontext(&thread->context, &dispatcher);
        return 0;
    }
    printf("ERROR: thread does not exist or already ended its execution!\n");
    return -1;
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



