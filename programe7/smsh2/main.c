#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "smsh.h"
#include "varlib.h"

#define DFL_PROMPT ">"


int main(){
	char *cmdline,*prompt,**arglist;
	prompt = DFL_PROMPT;
	int ret;
	
	setup();

	while((cmdline = next_cmd(prompt,stdin)) != NULL){
		arglist = splitline(cmdline);
		ret = process(arglist);
		freelist(arglist);
		free(cmdline);
	}

	return 0;
}

void setup(){
	extern char **environ;
	VLenviron2table(environ);

	signal(SIGINT,SIG_IGN);
	signal(SIGQUIT,SIG_IGN);
}

void fatal(char *s1,char *s2,int n){
	fprintf(stderr,"Error: %s,%s\n",s1,s2);
	exit(n);
}
