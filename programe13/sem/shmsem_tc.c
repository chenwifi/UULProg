#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include "command.h"

#define oops(m,x) {perror(m);exit(x);}

void wait_and_lock(int semset_id){
	struct sembuf actions[2];
	actions[0].sem_num = 1;
	actions[0].sem_op = 0;
	actions[0].sem_flg = SEM_UNDO;
	actions[1].sem_num = 0;
	actions[1].sem_op = 1;
	actions[1].sem_flg = SEM_UNDO;

	if(semop(semset_id,actions,2) == -1){
		oops("semop",1);
	}
}

void release_lock(int semset_id){
	struct sembuf actions[1];
	actions[0].sem_num = 0;
	actions[0].sem_op = -1;
	actions[0].sem_flg = SEM_UNDO;

	if(semop(semset_id,actions,1) == -1){
		oops("semop",1);
	}
}

int main(){
	int seg_id;
	timedata *p;
	int semset_id;
	key_t key;

	seg_id = get_ipc(sizeof(timedata));
	if(seg_id == -1){
		oops("shmget",1);
	}
	p = (timedata *)shmat(seg_id,NULL,0);
	if(p == (void *) -1 ){
		oops("shmat",1);
	}

	key = ftok("bigfat",56);
	semset_id = semget(key,2,IPC_CREAT | 0666);
	wait_and_lock(semset_id);

	printf("the time is %s",p->time);

	release_lock(semset_id);

	shmdt(p);

	return 0;
}
