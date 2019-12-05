#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM 5

int queue[NUM];
sem_t blank_num,product_num;

void *producer(void *p){
	static int x = 0;

	while(1){
		sem_wait(&blank_num);
		queue[x] = rand() % 1000;
		printf("produce %d\n",queue[x]);
		x = (x + 1) % NUM;
		sleep(rand() % 5);
		sem_post(&product_num);
	}
}

void *consumer(void *p){
	static int y = 0;

	while(1){
		sem_wait(&product_num);
		printf("consume %d\n",queue[y]);
		y = (y + 1) % NUM;
		sleep(rand() % 5);
		sem_post(&blank_num);
	}
}

int main(){
	pthread_t t1,t2;

	sem_init(&blank_num,0,NUM);
	sem_init(&product_num,0,0);
	pthread_create(&t1,NULL,producer,NULL);
	pthread_create(&t2,NULL,consumer,NULL);
	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
	sem_destroy(&blank_num);
	sem_destroy(&product_num);

	return 0;
}
