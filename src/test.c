#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>

#include "cdata.h"
#include "cthread.h"
#include "support.h"
#include "insert.h"

void twinkle();
void little();
void star();

int th1, th2, th3, th4;

void twinkle(){
	printf("twinkle ");
	cjoin(th2);
	cyield();
	printf("twinkle ");
	cyield();
}

void little(){
	printf("little ");
	cyield();
	printf("little ");
    cyield();
}

void star(){
	printf("star\n");
	cjoin(th3);
	printf("star!\n");
}


int main(){

    th1 = ccreate((void *)twinkle, (void *)NULL, 0);
	printf("creating twinkle thread with tid = %d\n", th1);

    th2 = ccreate((void *)twinkle, (void *)NULL, 0);
	printf("creating twinkle thread with tid = %d\n", th2);

    th3 = ccreate((void *)little, (void *)NULL, 0);
	printf("creating little thread with tid = %d\n", th3);

    th4 = ccreate((void *)star, (void *)NULL, 0);
	printf("creating star thread with tid = %d\n", th4);

    cjoin(th4);
    printf("the end.\n");
	return 0;
}
