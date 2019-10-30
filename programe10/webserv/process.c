#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "func.h"

void read_til_crnl(FILE *fp){
	char buf[BUFSIZ];

	while(fgets(buf,BUFSIZ,fp) != NULL && strcmp(buf,"\r\n") != 0){
		;
	}
}

void process_rq(char *rq,int fd){
	char cmd[BUFSIZ],arg[BUFSIZ];
	pid_t pid;

	if((pid = fork()) == -1){
		return ;
	}else if(pid > 0){
		wait(NULL);
	}

	strcpy(arg,"./");
	if(sscanf(rq,"%s%s",cmd,arg+2) != 2){
		return ;
	}

	if(strcmp(cmd,"GET") != 0){
		cannot_do(fd);
	}else if(not_exist(arg)){
		do_404(arg,fd);
	}else if(isadir(arg)){
		do_ls(arg,fd);
	}/*else if(ends_in_cgi(arg)){
		do_exec(arg,fd);
	}*/else{
		do_cat(arg,fd);
	}
}
