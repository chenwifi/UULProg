#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>

struct msg{
	struct msg *next;
	int num;
};

struct msg *head;
struct msg *tail;
pthread_cond_t has_product = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *consumer(void *p){
	struct msg *mp;

	while(1){
		pthread_mutex_lock(&lock);
		while(head == NULL){
			pthread_cond_wait(&has_product,&lock);
		}
		mp = head;
		if(head == tail){
			tail = mp->next;
		}
		head = mp->next;
		pthread_mutex_unlock(&lock);
		free(mp);
		printf("Consum %d\n",mp->num);
		sleep(rand() % 5);
	}
}

void *producer(void *p){
	struct msg *mp;

	while(1){
		mp = (struct msg *)malloc(sizeof(struct msg));
		mp->next = NULL;
		pthread_mutex_lock(&lock);
		if(head == NULL && tail == NULL){
			head = mp;
			tail = mp;
		}else{
			tail->next = mp;
			tail = mp;
		}
		mp->num = rand() % 1000;
		printf("Produce %d\n",mp->num);
		pthread_mutex_unlock(&lock);
		pthread_cond_signal(&has_product);
		sleep(rand() % 5);
	}
}

int main(){
	pthread_t t1,t2;

	srand(time(NULL));
	head = tail = NULL;
	pthread_create(&t1,NULL,producer,NULL);
	pthread_create(&t2,NULL,consumer,NULL);
	pthread_join(t1,NULL);
	pthread_join(t2,NULL);

	return 0;
}
