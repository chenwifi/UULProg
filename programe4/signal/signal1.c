#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <sys/types.h>

void new_op(int,siginfo_t*,void*);
void sig_op(int);

int main(int argc,char *argv[]){
	struct sigaction act;
	int sig;
	sig = atoi(argv[1]);

	
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = new_op;
	

	/*
	if(sigaction(sig,&act,NULL)){
		printf("install sigal error\n");
	}
	*/
	signal(sig,sig_op);
	
	if(sigaction(45,&act,NULL)){
		printf("install sigal error\n");
	}
	

	while(1){
		sleep(10);
		printf("wait for signal\n");
	}
	return 0;
}

void new_op(int signum,siginfo_t *info,void *myact){
	printf("receive signal %d\n",signum);
	//sleep(10);
	getchar();
	printf("newop\n");
}
void sig_op(int signum){
	printf("receive signal %d\n",signum);
	sleep(15);
	printf("sigop\n");
}
