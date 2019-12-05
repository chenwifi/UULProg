#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int myglobal;

void *thread_function(void *arg);

int main(){
	pthread_t mythread;
	int i;

	if(pthread_create(&mythread,NULL,thread_function,NULL)){
		printf("error creating thread\n");
		exit(1);
	}

	for(i = 0;i < 20;i++){
		myglobal = myglobal + 1;
		printf("o");
		fflush(stdout);
		sleep(1);
	}

	if(pthread_join(mythread,NULL)){
		printf("error joining thread\n");
		exit(1);
	}

	printf("myglobal : %d\n",myglobal);

	return 0;
}

void *thread_function(void *arg){
	int i,j;

	for(i = 0;i<20;i++){
		j = myglobal;
		j = j + 1;
		printf(".");
		fflush(stdout);
		sleep(1);
		myglobal = j;
	}

	return NULL;
}
