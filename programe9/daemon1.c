#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define oops(msg) {perror(msg);exit(1);}

void create_daemon();
int main(){
	time_t t;
	int fd;
	char *buf;

	create_daemon();
	while(1){
		fd = open("daemon.log",O_WRONLY|O_CREAT|O_APPEND,0644);
		if(fd == -1){
			oops("open");
		}
		t = time(NULL);
		buf = asctime(localtime(&t));
		write(fd,buf,strlen(buf));
		close(fd);
		sleep(60);
	}

	return 0;
}

void create_daemon(){
	pid_t pid;
	int i;

	pid = fork();
	if(pid == -1){
		oops("fork");
	}

	if(pid > 0){
		exit(EXIT_SUCCESS);
	}

	if(setsid() == -1){
		oops("setsid");
	}
	chdir("/");

	for(i = 0;i < 3;i++){
		close(i);
	}
	open("/dev/null",O_RDWR);
	dup(0);
	dup(0);
	umask(0);
	return ;
}
