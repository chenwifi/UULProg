#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

void new_op(int signum,siginfo_t *info,void *myact);

int main(){
	sigset_t new_mask,old_mask,pending_mask;
	struct sigaction act;

	sigemptyset(&act.sa_mask);
	act.sa_sigaction = new_op;
	act.sa_flags = SA_SIGINFO;

	if(sigaction(SIGRTMIN+10,&act,NULL)){
		printf("install signal error\n");
	}
	printf("num is %d\n",SIGRTMIN);

	sigemptyset(&new_mask);
	sigaddset(&new_mask,SIGRTMIN+10);

	if(sigprocmask(SIG_BLOCK,&new_mask,&old_mask)){
		printf("block error\n");
	}

	printf("now begin SIGRTMIN+10\n");
	sleep(15);
	
	if(sigpending(&pending_mask) < 0){
		printf("get pending error");
	}
	
	if(sigismember(&pending_mask,SIGRTMIN + 10)){
		printf("is pending\n");
	}

	if(sigprocmask(SIG_SETMASK,&old_mask,NULL)){
		printf("unblock signal error\n");
	}
	printf("signal unblock");
	fflush(stdout);
	sleep(15);
}

void new_op(int signum,siginfo_t *info,void *myact){
	printf("the int value is %d\n",signum);
}
