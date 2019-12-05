#include <stdio.h>
#include <pthread.h>

void *test(void *msg);

int main(){
	pthread_t t1,t2;

	pthread_create(&t1,NULL,test,"t1");
	pthread_create(&t2,NULL,test,"t2");
	pthread_join(t1,NULL);
	pthread_join(t2,NULL);

	return 0;
}

void *test(void *msg){
	printf("%s\n",msg);
	sleep(3);
	printf("%s\n",msg);
}
