#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "mysem.h"

int mysem_init(mysem *sem,unsigned int value){
	sem->value = value;
	if(pthread_mutex_init(&(sem->mutex),NULL) != 0){
		return 1;
	}
	if(pthread_cond_init(&(sem->cond),NULL) != 0){
		return 1;
	}

	return 0;
}

int mysem_wait(mysem *sem){
	pthread_mutex_lock(&(sem->mutex));
	while(sem->value <= 0){
		pthread_cond_wait(&(sem->cond),&(sem->mutex));
	}
	sem->value--;
	pthread_mutex_unlock(&(sem->mutex));

	return 0;
}

int mysem_post(mysem *sem){
	pthread_mutex_lock(&(sem->mutex));
	sem->value++;
	pthread_cond_broadcast(&(sem->cond));
	pthread_mutex_unlock(&(sem->mutex));

	return 0;
}

int mysem_destroy(mysem *sem){
	if(pthread_cond_destroy(&(sem->cond)) != 0){
		return 1;
	}
	if(pthread_mutex_destroy(&(sem->mutex)) != 0){
		return 1;
	}

	return 0;
}
