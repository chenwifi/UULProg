#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "queue.h"
#include "control.h"
#include "dbug.h"

#define NUM_WORKERS 4

int numthreads;

void *threadfunc(void *myarg);
struct work_queue{
	data_control control;
	queue work;
}wq;

typedef struct work_node{
	struct node *next;
	int jobnum;
}wnode;

struct cleanup_queue{
	data_control control;
	queue cleanup;
}cq;

typedef struct cleanup_node{
	struct node *next;
	int threadnum;
	pthread_t tid;
}cnode;

void initialize_structs(void){
	numthreads = 0;
	if(control_init(&wq.control)){
		dabort();
	}
	queue_init(&wq.work);
	if(control_init(&cq.control)){
		control_destroy(&wq.control);
		dabort();
	}
	queue_init(&cq.cleanup);
	control_activate(&wq.control);
}

int create_threads(){
	int x;
	cnode *curnode;

	for(x = 0;x < NUM_WORKERS;x++){
		curnode = (cnode *)malloc(sizeof(cnode));
		if(!curnode){
			return 1;
		}

		curnode->threadnum = x;
		if(pthread_create(&curnode->tid,NULL,threadfunc,(void *)curnode)){
			return 1;
		}

		printf("create thread %d\n",x);
		numthreads++;
	}

	return 0;
}

void *threadfunc(void *myarg){
	wnode *mywork;
	cnode *mynode;
	mynode = (cnode *)myarg;

	pthread_mutex_lock(&wq.control.mutex);
	while(wq.control.active){
		while(wq.work.head == NULL && wq.control.active){
			pthread_cond_wait(&wq.control.cond,&wq.control.mutex);
		}

		if(!wq.control.active){
			break;
		}

		mywork = (wnode *)queue_get(&wq.work);
		pthread_mutex_unlock(&wq.control.mutex);
		printf("Thread number %d processing job %d\n",mynode->threadnum,mywork->jobnum);
		free(mywork);
		sleep(1);
		pthread_mutex_lock(&wq.control.mutex);
	}
	pthread_mutex_unlock(&wq.control.mutex);
	pthread_mutex_lock(&cq.control.mutex);
	queue_put(&cq.cleanup,(node *)mynode);
	pthread_mutex_unlock(&cq.control.mutex);
	pthread_cond_signal(&cq.control.cond);
	printf("thread %d shutting down...\n",mynode->threadnum);

	return NULL;
}

void join_threads(){
	cnode *mynode;

	printf("join threads...\n");

	pthread_mutex_lock(&cq.control.mutex);
	while(numthreads){
		while(cq.cleanup.head == NULL){
			pthread_cond_wait(&cq.control.cond,&cq.control.mutex);
		}
		mynode = (cnode *)queue_get(&cq.cleanup);
		pthread_mutex_unlock(&cq.control.mutex);
		pthread_join(mynode->tid,NULL);
		numthreads--;
		free(mynode);
		printf("joined with thread %d\n",mynode->threadnum);
		pthread_mutex_lock(&cq.control.mutex);
	}
	pthread_mutex_unlock(&cq.control.mutex);
}

void cleanup_structs(){
	control_destroy(&wq.control);
	control_destroy(&cq.control);
}

void free_work(queue *work){
	wnode * mywork;

	if(work->head == NULL){
		return ;
	}

	while(work->head != NULL){
		mywork = (wnode *)queue_get(work);
		printf("free work by main thread...\n");
		free(mywork);
	}
}

int main(){
	int x;
	wnode *mywork;

	initialize_structs();

	if(create_threads()){
		printf("error starting threads\n");
		dabort();
	}

	pthread_mutex_lock(&wq.control.mutex);
	for(x = 0;x < 20;x++){
		mywork = (wnode *)malloc(sizeof(wnode));
		if(!mywork){
			printf("cannot malloc\n");
			break;
		}
		mywork->jobnum = x;
		queue_put(&wq.work,(node *)mywork);
	}
	pthread_mutex_unlock(&wq.control.mutex);
	pthread_cond_broadcast(&wq.control.cond);

	printf("sleeping...\n");
	sleep(2);
	printf("deactivating work queue...\n");
	control_deactivate(&wq.control);

	join_threads();
	cleanup_structs();
	free_work(&wq.work);

	return 0;
}
