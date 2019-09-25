#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define CHILD_MESS "i want a cookie\n"
#define PAR_MESS "testing...\n"

int main(){
	int pipefd[2];
	char buf[BUFSIZ];
	int len,read_len;

	if(pipe(pipefd) == -1){
		perror("pipe");
		exit(1);
	}

	switch(fork()){
		case -1:
			perror("fork");
			exit(1);
			break;
		case 0 :
			len = strlen(CHILD_MESS);
			while(1){
				if( write(pipefd[1],CHILD_MESS,len) != len){
					perror("write");
					exit(1);
				}
				sleep(5);
			}
		default :
			len = strlen(PAR_MESS);
			while(1){
				if( write(pipefd[1],PAR_MESS,len) != len ){
					perror("write");
					exit(1);
				}

				sleep(1);
				if((read_len = read(pipefd[0],buf,BUFSIZ)) <= 0){
					perror("read");
					exit(1);
				}
				write(1,buf,read_len);
			}
	}

	return 0;
}
