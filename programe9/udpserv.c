#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define BUF_SIZE 500

int main(int argc,char *argv[]){
	struct addrinfo hints;
	struct addrinfo *result,*rp;
	int s,sfd;
	socklen_t len;
	ssize_t nread;
	struct sockaddr_storage peer_addr;
	char buf[BUF_SIZE];

	if(argc != 2){
		fprintf(stderr,"Usage: %s port\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	memset(&hints,0,sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = 0;
	hints.ai_canonname = NULL;
	hints.ai_addr = NULL;
	hints.ai_next = NULL;

	s = getaddrinfo(NULL,argv[1],&hints,&result);
	printf("%s\n",argv[1]);
	if(s != 0){
		fprintf(stderr,"getaddrinfo: %s\n",gai_strerror(s));
		exit(EXIT_FAILURE);
	}

	for(rp = result;rp != NULL;rp = rp->ai_next){
		sfd = socket(rp->ai_family,rp->ai_socktype,rp->ai_protocol);
		if(sfd == -1){
			continue;
		}

		if(bind(sfd,rp->ai_addr,rp->ai_addrlen) == 0){
			break;
		}
		close(sfd);
	}

	if(rp == NULL){
		fprintf(stderr,"Could not bind\n");
		exit(EXIT_FAILURE);
	}

	freeaddrinfo(result);
	//printf("%s\n",rp->ai_addr->sa_data);
	/*
	char host[NI_MAXHOST],service[NI_MAXSERV];
	len = sizeof(struct sockaddr_storage);
	s = getnameinfo(rp->ai_addr,len,host,NI_MAXHOST,service,NI_MAXSERV,NI_NUMERICSERV);
	if(s == 0){
		printf("%s:%s\n",host,service);
	}else{
		printf("%s\n",gai_strerror(s));
	}
	*/

	while(1){
		len = sizeof(struct sockaddr_storage);
		nread = recvfrom(sfd,buf,BUF_SIZE,0,(struct sockaddr *)&peer_addr,&len);
		if(nread == -1){
			continue;
		}

		char host[NI_MAXHOST],service[NI_MAXSERV];
		s = getnameinfo((struct sockaddr *)&peer_addr,len,host,NI_MAXHOST,service,NI_MAXSERV,NI_NUMERICSERV);
		if(s == 0){
			printf("Received %ld bytes from %s:%s\n",(long)nread,host,service);
		}else{
			fprintf(stderr,"getnameinfo error:%s\n",gai_strerror(s));
		}

		if(sendto(sfd,buf,nread,0,(struct sockaddr *)&peer_addr,len) != nread){
			fprintf(stderr,"error sending response\n");
		}
	}

	return 0;
}
