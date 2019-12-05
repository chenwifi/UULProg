#include <stdio.h>
#include <pthread.h>

#define NUM 5

int count = 0;

void *print_count(void *m);
int main(){
	pthread_t t;
	int i;

	pthread_create(&t,NULL,print_count,NULL);
	for(i = 0;i < NUM;i++){
		count++;
		sleep(1);
	}
	pthread_join(t,NULL);

	return 0;
}

void *print_count(void *m){
	int i;
	for(i = 0 ; i < NUM ; i++){
		printf("%d\n",count);
		sleep(1);
	}
}
