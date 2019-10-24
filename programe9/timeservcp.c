#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define POSTNUM 13000
#define HOSTLEN 256
#define oops(msg) {perror(msg);exit(1);}

int main(){
	int sock_id,sock_fd;
	struct sockaddr_in saddr;
	char hostname[HOSTLEN];
	struct hostent *hp;
	FILE *sock_fp;
	time_t t;

	sock_id = socket(AF_INET,SOCK_STREAM,0);
	if(sock_id == -1){
		oops("socket");
	}

	bzero((void *)&saddr,sizeof(saddr));
	gethostname(hostname,HOSTLEN);
	hp = gethostbyname(hostname);

	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(POSTNUM);
	//saddr.sin_port = POSTNUM;
	bcopy((void *)hp->h_addr,(void *)&saddr.sin_addr,hp->h_length);
	if(bind(sock_id,(struct sockaddr *)&saddr,sizeof(saddr)) != 0){
		oops("bind");
	}

	if(listen(sock_id,1) != 0){
		oops("listen");
	}
	
	while(1){
		sock_fd = accept(sock_id,NULL,NULL);
		printf("wow! get a call!!\n");
		if(sock_fd == -1){
			oops("accpet");
		}

		sock_fp = fdopen(sock_fd,"w");
		if(sock_fp == NULL){
			oops("fdopen");
		}
		t = time(NULL);

		fprintf(sock_fp,"The time here is ... \n");
		fprintf(sock_fp,"%s\n",ctime(&t));
		fclose(sock_fp);
	}

	return 0;
}
