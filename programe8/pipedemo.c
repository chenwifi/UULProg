#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int main(){
	int pipefd[2];
	int i,len;
	char buf[BUFSIZ];
	
	if(pipe(pipefd) == -1){
		perror("could not make pipe");
		exit(1);
	}

	printf("got a pipe and fds are %d,%d\n",pipefd[0],pipefd[1]);

	while(fgets(buf,BUFSIZ,stdin)){
		len = strlen(buf);

		if(write(pipefd[1],buf,len) != len){
			perror("write to pipe");
			exit(1);
		}

		for(i = 0;i < len;i++){
			buf[i] = 'X';
		}

		if(read(pipefd[0],buf,BUFSIZ) <= 0){
			perror("read from pipe");
			exit(1);
		}

		if(write(1,buf,len) != len){
			perror("write to stdout");
			exit(1);
		}
	}

	return 0;
}
