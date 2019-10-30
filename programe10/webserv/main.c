#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include "process.h"
#include "socklib.h"

int main(int argc,char *argv[]){
	int sock,fd;
	FILE *fpin;
	char request[BUFSIZ];

	if(argc != 2){
		fprintf(stderr,"usage:%s portnum\n",argv[0]);
		exit(1);
	}

	sock = make_server_socket(atoi(argv[1]));
	if(sock == -1){
		exit(1);
	}
	
	while(1){
		fd = accept(sock,NULL,NULL);
		fpin = fdopen(fd,"r");
		fgets(request,BUFSIZ,fpin);
		printf("get a call: request = %s\n",request);
		read_til_crnl(fpin);
		process_rq(request,fd);
		fclose(fpin);
	}

	return 0;
}
