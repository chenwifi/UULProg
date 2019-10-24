#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define READ 0
#define WRITE 1

FILE *mypopen(char *command,char *mode);

int main(){
	FILE *fp;
	/*
	char buf[100];
	int i = 0;

	fp = mypopen("who|sort","r");

	while(fgets(buf,100,fp) != NULL){
		printf("%3d %s",i++,buf);
	}
	pclose(fp);
	*/
	//execl("/bin/sh","sh","-c","bc",NULL);

	fp = mypopen("bc","w");
	//while(1){
		fprintf(fp,"2+3\n");
	//}
	sleep(5);
	fclose(fp);
	wait(NULL);
	//pclose(fp);
	sleep(10);

	return 0;
}

FILE *mypopen(char *command,char *mode){
	int parent_close,child_close;
	pid_t pid;
	int pipefd[2];

	if(*mode == 'r'){
		parent_close = WRITE;
		child_close = READ;
	}else if(*mode == 'w'){
		parent_close = READ;
		child_close = WRITE;
	}else{
		perror("mode error");
		exit(1);
	}

	if(pipe(pipefd) == -1){
		perror("pipe");
		exit(1);
	}

	if((pid = fork()) == -1){
		perror("fork");
		exit(1);
	}

	if(pid == 0){
		close(pipefd[child_close]);

		/*
		if(child_close == READ){
			dup2(pipefd[parent_close],1);
		}else{
			dup2(pipefd[parent_close],0);
		}
		close(pipefd[parent_close]);
		*/

		//change to the following

		dup2(pipefd[parent_close],parent_close);
		close(pipefd[parent_close]);

		execl("/bin/sh","sh","-c",command,NULL);
		//execl("/bin/sh","sh",command,NULL);

		perror("execlp");
		exit(1);

	}else{
		printf("child id %d\n",pid);
		close(pipefd[parent_close]);

		return fdopen(pipefd[child_close],mode);
	}
}
