#include <stdio.h>
#include <pthread.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t flag = PTHREAD_COND_INITIALIZER;

void * threadfunc(void *arg){
	pthread_mutex_lock(&lock);
	pthread_cond_signal(&flag);
	pthread_mutex_unlock(&lock);
}

int main(){
	pthread_t t1;
	
	pthread_mutex_lock(&lock);
	pthread_create(&t1,NULL,threadfunc,NULL);
	pthread_cond_wait(&flag,&lock);
	pthread_join(t1,NULL);
	pthread_cond_signal(&flag);
	//pthread_mutex_lock(&lock);
	printf("ok\n");

	return 0;
}
