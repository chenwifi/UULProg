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
	char buf[BUFSIZ];

	pipefd = open("bigfat",O_RDONLY);
	if(pipefd != -1){
		res = read(pipefd,buf,BUFSIZ);
		if(res > 0){
			write(1,buf,res);
		}
	}

	printf("Process %d finish\n",getpid());

	return 0;
}
