#include <stdio.h>
#include "cdata.h"
#include "cthread.h"
#include "support.h"
#include "insert.h"

void quick();
int th1;

void quick(){
	printf("Hey, I'm a quick thread, little CPU time\n");
	cyield();
	printf("I'm here again\n");
	cyield();
	while(1){
	    printf("and again!\n");
	    cyield();
	}
}


int main(){
    int i;

    th1 = ccreate((void *)quick, (void *)NULL, 0);
	printf("creating a quick thread with tid = %d\n", th1);


	for(i = 0; i < 100000; i++){
	    i++;
	    i--;
	    i = i;
	}

	cyield();

	printf("I'm the Main Thread an I took a lot to execute, so I'm the last one :( \n");

	return 0;
}
