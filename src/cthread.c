
#include <stdio.h>
#include "../include/cthread.h"


int cidentify (char *name, int size){
    int printSize;
    char names[] = "Felipe de Almeida Graeff - 00261606\nJulia Eidelwein - 00274700\n";
    printSize = snprintf(name,size,"Group:\n%s",names);
    if(printSize == 70 || printSize == size){
        return 0;
    }
    return -1;
}



