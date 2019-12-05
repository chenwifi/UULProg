#include <stdio.h>
#include <stdlib.h>

void do_regular_work();

int main(int argc,char *argv[]){
	setup();
	if(get_ticket() != 0){
		exit(1);
	}

	do_regular_work();

	release_ticket();
	shut_down();

	return 0;
}

void do_regular_work(){
	printf("SuperSleep version 1.0 Running -Licensed Software\n");
	sleep(10);
}
