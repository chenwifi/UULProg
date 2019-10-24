#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define POSTNUM 13000
#define HOSTLEN 256

int main(){
	int sock_id,sock_fd;
	struct hostent *hp;
	struct sockaddr_in saddr;
	char hostname[HOSTLEN];

	sock_id = socket(AF_INET,SOCK_STREAM,0);
	if(sock_id == -1){
		perror("socket");
		exit(1);
	}
	
	bzero((void *)&saddr,sizeof(saddr));
	gethostname(hostname,HOSTLEN);
	hp = gethostbyname(hostname);
	bcopy((void *)hp->h_addr,(void *)&saddr.sin_addr,hp->h_length);
	saddr.sin_port = htons(POSTNUM);
	saddr.sin_family = AF_INET;

	if(bind(sock_id,(struct sockaddr *)&saddr,sizeof(saddr)) != 0){
		perror("bind");
		exit(1);
	}

	if(listen(sock_id,1) != 0){
		perror("listen");
		exit(1);
	}

	while(1){
		sock_fd = accept(sock_id,NULL,NULL);
		printf("wow!!got a call!\n");
	}

	return 0;
}
