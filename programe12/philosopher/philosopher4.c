#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

#define N 5

sem_t chopsticks[N];
int philosophers[N] = {0,1,2,3,4};
pthread_mutex_t mutex;

sem_t m;

void *phi(void *arg){
	int i = *(int *)arg;
	int left = i;
	int right = (i + 1) % N;

	while(1){
		printf("philosopher %d is thinking\n",i);
		sleep(3);
		printf("philosopher %d is hungry\n",i);

		sem_wait(&m);

		sem_wait(&chopsticks[left]);
		printf("philo %d get one  %d chopstick,can not eat\n",i,left);
		sleep(3);
		sem_wait(&chopsticks[right]);
		printf("philo %d get two %d\n",i,right);

		sem_post(&m);


		printf("philosopher %d is eatting\n",i);
		sleep(3);

		sem_post(&chopsticks[left]);
		printf("philo %d put down %d chopsticks\n",i,left);
		sem_post(&chopsticks[right]);
		printf("philo %d put down %d chopsticks\n",i,right);
	}
}

int main(){
	int i;
	pthread_t philo[N];

	for(i = 0;i < N;i++){
		sem_init(&chopsticks[i],0,1);
	}
	pthread_mutex_init(&mutex,NULL);

	sem_init(&m,0,N-1);

	for(i = 0;i < N;i++){
		pthread_create(&philo[i],NULL,phi,&philosophers[i]);
	}

	for(i = 0;i < N;i++){
		pthread_join(philo[i],NULL);
	}

	for(i = 0;i < N;i++){
		sem_destroy(&chopsticks[i]);
	}
	pthread_mutex_destroy(&mutex);

	sem_destroy(&m);

	return 0;
}
