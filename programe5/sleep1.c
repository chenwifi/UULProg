#include <stdio.h>
#include <signal.h>

void wakeup(int);

int main(){
	int i,b;
	printf("about sleep for 4 seconds\n");
	signal(SIGALRM,wakeup);
	alarm(4);
	//pause();
	while(1){
	}
	printf("wake up\n");

	return 0;
}

void wakeup(int signum){
	printf("Alarm receive from kernel \n");
}
