#include <stdio.h>
#include "../include/cthread.h"

csem_t sem;

void* f1(void* args){
    printf("Inicio da thread 1\n");
    cwait(&sem);
    printf("Thread 1 passando semaforo\n");
    cyield();
    csignal(&sem);
    printf("Fim da thread 1\n");
    return NULL;
}

void* f2(void* args){
    printf("Inicio da thread 2\n");
    cwait(&sem);
    printf("Thread 2 passando semaforo\n");
    cyield();
    csignal(&sem);
    printf("Fim da thread 2\n");
    return NULL;
}

void* f3(void* args){
    printf("Inicio da thread 3\n");
    cwait(&sem);
    printf("Thread 3 passando semaforo\n");
    cyield();
    csignal(&sem);
    printf("Fim da thread 3\n");
    return NULL;
}

int main(){
    csem_init(&sem, 1);

    int th1 = ccreate(f1,NULL,0);
    printf("thread %d created\n",th1);
    int th2 = ccreate(f2,NULL,0);
    printf("thread %d created\n",th2);
    int th3 = ccreate(f3,NULL,0);
    printf("thread %d created\n",th3);
    
    cjoin(th1);
    cjoin(th2);
    cjoin(th3);

    return 0;
}

