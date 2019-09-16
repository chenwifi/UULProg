#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

void new_op(int,siginfo_t*,void*);

int main(int argc,char *argv[]){
	struct sigaction act;
	union sigval mysigval;
	int i;
	int sig;
	pid_t pid;
	char data[10];
	memset(data,0,sizeof(data));

	for(i=0;i<5;i++){
		data[i] = '2';
	}
	mysigval.sival_ptr = data;

	sig = atoi(argv[1]);
	pid = getpid();
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = new_op;

	if(sigaction(sig,&act,NULL) < 0){
		printf("install sigal error\n");
	}

	while(1){
		sleep(2);
		printf("wait for the signal\n");
		sigqueue(pid,sig,mysigval);
	}

	return 0;
}

void new_op(int sig,siginfo_t *info,void *myact){
	int i;
	printf("%s",info->si_value.sival_ptr);
	/*
	for(i=0;i<10;i++){
		printf("%c\n",(*((char *)(info->si_ptr+i))));
	}
	*/
	printf("handle signal %d over\n",sig);
}
