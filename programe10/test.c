#include <stdio.h>
#include <signal.h>

void myfunc(int);
void myfunc2(int,siginfo_t *,void *);
int main(){
	struct sigaction act;

	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = myfunc2();
	//printf("%d\n",SIGRTMIN);
	//signal(44,myfunc);
	while(1){
		;
	}

	return 0;
}

void myfunc2(int signum,siginfo_t *info,void *myact){
	printf("re");
}

void myfunc(int sig){
	printf("start----\n");
	sleep(10);
	printf("end----\n");
}
