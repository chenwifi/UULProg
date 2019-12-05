#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>

void *threadfunc(void *msg){
	pid_t pid;
	int status;

	if((pid = fork()) == -1){
		perror("fork");
		exit(1);
	}

	if(pid == 0){
		printf("get process id:%d\n",getpid());
		sleep(2);
	}else if(pid > 0){
		printf("this is father process\n");
		wait(&status);
		sleep(10);
	}
	printf("test2--%d\n",getpid());
}

int main(){
	pthread_t t1,t2;
	int stat;

	pthread_create(&t1,NULL,threadfunc,NULL);
	//sleep(3);
	pthread_create(&t2,NULL,threadfunc,NULL);
	printf("test1\n");
	stat = pthread_join(t1,NULL);
	printf("%d\n",stat);
	stat = pthread_join(t1,NULL);
	printf("%d\n",stat);
	stat = pthread_join(t2,NULL);
	printf("%d\n",stat);
	stat = pthread_join(t2,NULL);
	printf("%d\n",stat);

	printf("before sleep\n");
	sleep(5);
	printf("after sleep\n");

	return 0;
}
