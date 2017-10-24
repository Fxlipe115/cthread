#include <stdio.h>
#include <stdlib.h>

#include "cthread.h"

void sumValue();
void subValue();
void divValue();
void multValue();
void absValue();
void powValue();
csem_t sem;

int th1, th2, th3, th4;
int value = 10;


void sumValue(){
    value = value + 5;
    printf("+ 5 = %d - cyield\n", value);
    cyield();
    value = value + 3;
    printf("+ 3 = %d - cyield\n", value);
    cyield();
    value = value + 5;
    printf("+ 5 = %d - cjoin(div) - won't join\n", value);
    cjoin(th4);
    value = value + 1;
    printf("+ 1 = %d - end sum thread\n", value);
}
void subValue(){
    value = value - 4;
    printf("- 4 = %d - cwait - continue\n", value);
    cwait(&sem);
    value = value - 1;
    printf("- 1 = %d - cyield\n", value);
    cyield();
    value = value - 9;
    printf("- 9 = %d - cyield\n", value);
    cyield();
    printf("sub made a csignal and cjoin(sum)\n");
    csignal(&sem);
    cjoin(th1);
    value = value - 11;
    printf("- 5 = %d - end sub thread\n", value);
}
void divValue(){
    value = (int)value/2;
    printf("/ 2 = %d - cwait -> BLOCKED\n", value);
    cwait(&sem);
    value = (int)value/4;
    printf("/ 4 = %d - end div thread\n", value);
}
void multValue(){
    value = value*3;
    printf("* 3 = %d - cjoin(div)\n", value);
    cjoin(th3);
    value = value*5;
    printf("* 5 = %d - end mult thread\n", value);

}


int main(){
    csem_init(&sem, 1);

    th1 = ccreate((void *)sumValue, (void *)NULL, 0);
	printf("creating sum thread with tid = %d\n", th1);

    th2 = ccreate((void *)subValue, (void *)NULL, 0);
	printf("creating sub thread with tid = %d\n", th2);

    th3 = ccreate((void *)divValue, (void *)NULL, 0);
	printf("creating div thread with tid = %d\n", th3);

    th4 = ccreate((void *)multValue, (void *)NULL, 0);
	printf("creating mult thread with tid = %d\n", th4);

	printf("initial value = %d\n", value);

    cjoin(th2);
    printf("value = %d\n", value);
	return 0;
}
