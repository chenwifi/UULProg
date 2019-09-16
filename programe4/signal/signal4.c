#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

int main(int argc,char **argv){
	pid_t pid;
	int signum;
	union sigval mysigval;

	signum = atoi(argv[1]);
	pid = (pid_t)atoi(argv[2]);
	mysigval.sival_int = 66;

	if(sigqueue(pid,signum,mysigval) < 0){
		printf("send message error\n");
	}
	sleep(2);
}
