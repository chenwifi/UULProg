#include <stdio.h>
#include <pthread.h>
#include "mysem.h"

#define NUM 5

int queue[NUM];
mysem blank_num,product_num;

void *producer(void *p){
	static int x = 0;

	while(1){
		mysem_wait(&blank_num);
		queue[x] = rand() % 1000;
		printf("produce %d\n",queue[x]);
		x = (x + 1) % NUM;
		sleep(rand() % 5);
		mysem_post(&product_num);
	}
}

void *consumer(void *p){
	static int y = 0;

	while(1){
		mysem_wait(&product_num);
		printf("consume %d\n",queue[y]);
		y = (y + 1) % NUM;
		sleep(rand() % 5);
		mysem_post(&blank_num);
	}
}

int main(){
	pthread_t t1,t2;

	mysem_init(&blank_num,NUM);
	mysem_init(&product_num,0);
	pthread_create(&t1,NULL,producer,NULL);
	pthread_create(&t2,NULL,consumer,NULL);
	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
	mysem_destroy(&blank_num);
	mysem_destroy(&product_num);

	return 0;
}
