#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

pthread_t ntid;

void printids(char *s){
	pid_t pid;
	pthread_t tid,tid2;

	pid = getpid();
	tid = pthread_self();
	tid2 = ntid;

	printf("%s pid %u tid %u tid2 %u\n",s,pid,tid,tid2);
}
void *thr_fn(void *arg){
	printids((char *)arg);
	return NULL;
}

int main(){
	int err;
	err = pthread_create(&ntid,NULL,thr_fn,"new thread: ");

	if(err != 0){
		fprintf(stderr,"cannot create thread :%s\n",strerror(err));
		exit(1);
	}

	printids("main thread:");
	sleep(2);

	return 0;
}
