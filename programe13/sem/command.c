#include <stdio.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include "command.h"

int sharememory(int size,int flag){
	int id;
	key_t key = ftok("bigfat",69);

	if(key < 0){
		fprintf(stderr,"ftok error");
		return -1;
	}

	id = shmget(key,size,flag);
	if(id < 0){
		fprintf(stderr,"shmget error");
		return -1;
	}

	return id;
}

int creat_ipc(int size){
	return sharememory(size,IPC_CREAT | IPC_EXCL | 0666);
}

int get_ipc(int size){
	return sharememory(size,IPC_CREAT | 0666);
}

int destroy_sharememory(int id){
	return shmctl(id,IPC_RMID,NULL);
}
