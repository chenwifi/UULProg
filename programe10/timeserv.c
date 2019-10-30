#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "socklib.c"

#define PORTNUM 13000

void process_request(int fd);
void process_request_fork(int fd);
void test(int);
int main(){
	int sock,fd;
	sock = make_server_socket(PORTNUM);
	if(sock == -1){
		exit(1);
	}
	signal(44,test);

	while(1){
		printf("start--\n");
		fd = accept(sock,NULL,NULL);
		if(fd == -1){
			printf("%d--%d\n",errno,EINTR);
			break;
		}
		printf("go on...\n");
		//process_request(fd);
		process_request_fork(fd);
		close(fd);
	}

	return 0;
}

void process_request_fork(int fd){
	pid_t pid;
	pid = fork();

	if(pid == -1){
		exit(1);
	}

	if(pid == 0){
		dup2(fd,1);
		close(fd);
		execl("/bin/date","date",NULL);
	}else{
		wait(NULL);
	}
}

void test(int sig){
	printf("received signum %d\n",sig);
}

void process_request(int fd){
	time_t t;
	char *cp;

	time(&t);
	cp = ctime(&t);
	write(fd,cp,strlen(cp));
}
