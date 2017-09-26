
#include <stdio.h>
#include <string.h>
#include "../include/cthread.h"


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

int ccreate (void* (*start)(void*), void *arg, int prio){
    //TODO
}

int cyield(void){
    //TODO
}

int cjoin(int tid){
    //TODO
}

