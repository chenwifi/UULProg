#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include "command.h"

#define oops(m,x) {perror(m);exit(x);}

union semun{
	int val;
	struct semid_ds *buf;
	unsigned short *array;
	struct seminfo  *__buf;
};

int seg_id;
int semset_id;

/*
void cleanup(){
	shmctl(seg_id,IPC_RMID,NULL);
	semctl(semset_id,0,IPC_RMID,NULL);
}
*/

void set_sem_value(int semset_id,int semnum,int val){
	union semun initval;
	initval.val = val;

	if(semctl(semset_id,semnum,SETVAL,initval) == -1){
		oops("semctl",1);
	}
}

//the first of the set is reader,the second is write
void wait_and_lock(){
	struct sembuf actions[3];
	actions[0].sem_num = 0;
	actions[0].sem_op = 0;
	actions[0].sem_flg = SEM_UNDO;
	actions[1].sem_num = 1;
	actions[1].sem_op = 0;
	actions[1].sem_flg = SEM_UNDO;
	actions[2].sem_num = 1;
	actions[2].sem_op = 1;
	actions[2].sem_flg = SEM_UNDO;

	if(semop(semset_id,actions,3) == -1){
		oops("semop",1);
	}
}

void release_lock(){
	struct sembuf actions[1];
	actions[0].sem_num = 1;
	actions[0].sem_op = -1;
	actions[0].sem_flg = SEM_UNDO;

	if(semop(semset_id,actions,1) == -1){
		oops("semop",1);
	}
}

int main(){
	timedata *p;
	int n;
	time_t now;
	char *message;
	key_t key;

	seg_id = creat_ipc(sizeof(timedata));
	if(seg_id == -1){
		oops("shmget",1);
	}

	p = (timedata *)shmat(seg_id,NULL,0);
	if(p == (void *) -1 ){
		oops("shmat",1);
	}

	key = ftok("bigfat",56);
	semset_id = semget(key,2,IPC_CREAT | IPC_EXCL | 0666);
	if(semset_id == -1){
		oops("semget",1);
	}
	set_sem_value(semset_id,0,0);
	set_sem_value(semset_id,1,0);

	for(n = 0; n < 20;n++){
		time(&now);
		message = ctime(&now);
		
		wait_and_lock();
		printf("getting lock and updating memory\n");

		strcpy(p->time,message);

		release_lock();

		sleep(1);
	}
	shmdt(p);

	//cleanup();

	return 0;
}
