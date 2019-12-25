#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include "command.h"

int main(){
	timedata *p;
	char *tmp;
	time_t now;
	int id = creat_ipc(sizeof(timedata));

	if(id < 0){
		fprintf(stderr,"creat ipc error");
		exit(1);
	}
	p = (timedata *)shmat(id,NULL,0);
	if(p < 0){
		fprintf(stderr,"shmat error");
		exit(1);
	}

	while(1){
		time(&now);
		char *tmp = ctime(&now);
		strcpy(p->time,tmp);
		sleep(1);
		if(p->from == 4){
			break;
		}
	}

	if(shmdt(p) < 0){
		fprintf(stderr,"shmdt error");
		exit(1);
	}

	if(destroy_sharememory(id) < 0){
		fprintf(stderr,"destroy error");
		exit(1);
	}

	return 0;
}
