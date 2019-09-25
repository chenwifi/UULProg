#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc,char *argv[]){
	int pipefd[2];
	pid_t cpid;
	int newfd;
	char buf[BUFSIZ];
	int read_len;

	if(argc != 3){
		perror("args error");
		exit(1);
	}

	if(pipe(pipefd) == -1){
		perror("pipe");
		exit(1);
	}

	cpid = fork();

	if(cpid == -1){
		perror("fork");
		exit(1);
	}
	
	if(cpid == 0){
		close(pipefd[0]);
		if((newfd = dup2(pipefd[1],1)) == -1){
			perror("redir io error");
			exit(1);
		}
		close(pipefd[1]);
		sleep(5);
		execlp(argv[1],argv[1],NULL);
	}else{
		close(pipefd[1]);
		/*
		if((newfd = dup2(pipefd[0],0)) == -1){
			perror("redir io error");
			exit(1);
		}
		*/
		//close(pipefd[0]);
		//execlp(argv[2],argv[2],NULL);

		while((read_len = read(pipefd[0],buf,BUFSIZ)) > 0){
			write(1,buf,read_len);
			printf("---");
		}
	}

	return 0;
}
