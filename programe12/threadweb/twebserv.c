#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include "socklib.h"
#include "func.h"

int server_requests;
int server_bytes_sent;
time_t server_started;

void setup(pthread_attr_t *attr);
void *handle_call(void *);
void skip_rest_of_header(FILE *fp);
void process_rq(char *rq,int fd);

int main(int argc,char *argv[]){
	int sock,fd;
	int *fdptr;
	pthread_attr_t attr;
	pthread_t t;

	if(argc != 2){
		fprintf(stderr,"usage: %s portnum",argv[0]);
		exit(1);
	}

	sock = make_server_socket(atoi(argv[1]));
	if(sock == -1){
		perror("making socket");
		exit(1);
	}

	setup(&attr);

	while(1){
		fd = accept(sock,NULL,NULL);
		server_requests++;
		fdptr = (int *)malloc(sizeof(int));
		*fdptr = fd;
		pthread_create(&t,&attr,handle_call,fdptr);
	}

	return 0;
}

void setup(pthread_attr_t *attr){
	pthread_attr_init(attr);
	pthread_attr_setdetachstate(attr,PTHREAD_CREATE_DETACHED);

	server_requests = 0;
	server_bytes_sent = 0;
	time(&server_started);
}

void *handle_call(void *fdptr){
	int fd;
	char request[BUFSIZ];
	FILE *fpin;

	fd = *(int *)fdptr;
	free(fdptr);

	fpin = fdopen(fd,"r");
	fgets(request,BUFSIZ,fpin);
	printf("got a call on %d : request = %s\n",fd,request);

	skip_rest_of_header(fpin);
	process_rq(request,fd);

	fclose(fpin);
}

void skip_rest_of_header(FILE *fp){
	char buf[BUFSIZ];

	while(fgets(buf,BUFSIZ,fp) != NULL && strcmp(buf,"\r\n") != 0){
		;
	}
}

void process_rq(char *rq,int fd){
	char cmd[BUFSIZ],arg[BUFSIZ];

	if(sscanf(rq,"%s%s",cmd,arg) != 2){
		return ;
	}

	printf("cmd and arg are %s--%s\n",cmd,arg);

	if(strcmp(cmd,"GET") != 0){
		not_implemented(fd);
	}else if(built_in(arg,fd)){
		;
	}else if(not_exist(arg)){
		do_404(arg,fd);
	}else if(isadir(arg)){
		do_ls(arg,fd);
	}else{
		do_cat(arg,fd);
	}
}
