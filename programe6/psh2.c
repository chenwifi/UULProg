#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

#define MAXARGS 20
#define ARGLEN 100

int execute(char *arglist[]);
char *makestring(char *buf);

int main(){
	char *arglist[MAXARGS + 1];
	int numargs;
	char argbuf[ARGLEN];
	numargs = 0;

	while(numargs < MAXARGS){
		printf("Arg[%d] ?",numargs);
		if(fgets(argbuf,ARGLEN,stdin) && *argbuf != '\n'){
			arglist[numargs++] = makestring(argbuf);
		}else{
			if(numargs > 0){
				arglist[numargs] = NULL;
				execute(arglist);
				numargs = 0;
			}
		}
	}

	return 0;
}

int execute(char *arglist[]){
	int status,pid;

	pid = fork();
	switch(pid){
		case -1:
			perror("fork");
			exit(1);
			break;
		case 0:
			if(execvp(arglist[0],arglist)){
				perror("execvp error");
				exit(1);
			}
			exit(0);
			break;
		default:
			signal(SIGINT,SIG_IGN);
			while(wait(&status) != pid)
				;
			if(WIFEXITED(status)){
				printf("child exit with status:%d\n",WEXITSTATUS(status));
			}
			break;
	}
}

char *makestring(char *buf){
	char *cp;
	buf[strlen(buf)-1] = '\0';
	cp = (char *)malloc(strlen(buf) + 1);
	if(cp == NULL){
		fprintf(stderr,"no memory\n");
		exit(1);
	}

	strcpy(cp,buf);
	return cp;
}
