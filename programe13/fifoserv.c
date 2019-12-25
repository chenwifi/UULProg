#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(){
	int res;
	int pipefd;
	char buf[BUFSIZ] = "hello world!!!";

	if(access("bigfat",F_OK) != 0){
		res = mkfifo("bigfat",0777);
		if(res == -1){
			fprintf(stderr,"mkfifo error");
			exit(1);
		}
	}

	printf("Process %d opening FIFO\n",getpid());

	pipefd = open("bigfat",O_WRONLY);
	if(pipefd != -1){
		res = write(pipefd,buf,strlen(buf));
		if(res == -1){
			fprintf(stderr,"write error on pipe\n");
			exit(1);
		}
	}else{
		exit(1);
	}

	printf("Process %d finish\n",getpid());

	return 0;
}
