#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include "command.h"

int main(){
	int id;
	timedata *p;

	id = get_ipc(sizeof(timedata));
	if(id < 0){
		fprintf(stderr,"creat ipc error");
		exit(1);
	}

	p = (timedata *)shmat(id,NULL,0);
	if(p < 0){
		fprintf(stderr,"shmat error");
		exit(1);
	}

	printf("get the time %s\n",p->time);
	p->from = 4;

	if(shmdt(p) < 0){
		fprintf(stderr,"shmdt error");
		exit(1);
	}

	return 0;
}
