#include <stdio.h>
#include <pthread.h>

void *pthread_run1(void *arg){
	while(1){
		printf("i am thread 1,ID:%d\n",pthread_self());
		sleep(10);
	}
}

void *pthread_run2(void *arg){
	while(1){
		printf("i am thread 2,ID:%d\n",pthread_self());
		sleep(10);
	}
}

int main(){
	pthread_t t1,t2;

	pthread_create(&t1,NULL,pthread_run1,NULL);
	pthread_create(&t2,NULL,pthread_run2,NULL);

	printf("i am main pthread\n");

	pthread_join(t1,NULL);
	pthread_join(t2,NULL);

	return 0;
}
