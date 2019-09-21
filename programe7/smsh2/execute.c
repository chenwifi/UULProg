#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include "varlib.h"

extern char **environ;

int execute(char *arglist[]){
	int status,pid;

	pid = fork();
	switch(pid){
		case -1:
			perror("fork");
			exit(1);
			break;
		case 0:
			environ = VLtable2environ();
			signal(SIGINT,SIG_DFL);
			signal(SIGQUIT,SIG_DFL);
			if(execvp(arglist[0],arglist)){
				perror("execvp error");
				exit(1);
			}
			exit(0);
			break;
		default:
			while(wait(&status) != pid)
				;
			if(WIFEXITED(status)){
				return WEXITSTATUS(status);
			}
			break;
	}

}
