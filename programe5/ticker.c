#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>

void countdown(int);
int set_ticker(int);

int main(){
	signal(SIGALRM,countdown);

	if(set_ticker(500) == -1){
		perror("set ticker");
	}else{
		while(1){
			pause();
		}
	}

	return 0;
}

int set_ticker(int secs){
	struct itimerval new_timeset;
	long n_sec,n_usec;

	n_sec = secs / 1000;
	n_usec = (secs % 1000) * 1000;

	new_timeset.it_value.tv_sec = n_sec;
	new_timeset.it_value.tv_usec = n_usec;
	new_timeset.it_interval.tv_sec = n_sec;
	new_timeset.it_interval.tv_usec = n_usec;

	return setitimer(ITIMER_REAL,&new_timeset,NULL);
}

void countdown(int signum){
	static int num = 10;
	printf("%d--",num--);
	fflush(stdout);

	sleep(5);

	if(num < 0){
		printf("done\n");
		exit(0);
	}
}
