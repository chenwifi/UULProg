#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(){
	int fd,pid,status,cfd;
	fd = open("xyz",O_RDONLY);
	printf("fd:%d\n",fd);

	pid = fork();

	if(pid == 0){
		cfd = open("xyz",O_RDONLY);
		printf("cfd:%d\n",cfd);
		execlp("./cfdtest","./cfdtest",NULL);
	}else{
		wait(&status);
		printf("mainfd:%d\n",open("xyz",O_RDONLY));
	}

	return 0;
}
