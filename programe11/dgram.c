#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <strings.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#define HOSTLEN 256

int make_internet_address(char *host,int port,struct sockaddr_in *addrp);

int make_dgram_server_socket(int portnum){
	int sock_id;
	char hostname[HOSTLEN];
	struct sockaddr_in saddr;

	sock_id = socket(AF_INET,SOCK_DGRAM,0);
	if(sock_id == -1){
		return -1;
	}

	gethostname(hostname,HOSTLEN);
	make_internet_address(hostname,portnum,&saddr);

	if(bind(sock_id,(struct sockaddr *)&saddr,sizeof(saddr)) != 0){
		return -1;
	}

	return sock_id;
}

int make_internet_address(char *host,int port,struct sockaddr_in *addrp){
	struct hostent *hp;

	hp = gethostbyname(host);
	if(hp == NULL){
		return -1;
	}

	bcopy((void *)hp->h_addr,(void *)&addrp->sin_addr,hp->h_length);
	addrp->sin_port = htons(port);
	addrp->sin_family = AF_INET;

	return 0;
}

int get_internet_address(char *host,int len,int *portp,struct sockaddr_in *addrp){
	char *tmp;
	tmp = inet_ntoa(addrp->sin_addr);
	*portp = ntohs(addrp->sin_port);
	strcpy(host,tmp);

	return 0;
}

int make_dgram_client_socket(){
	return socket(AF_INET,SOCK_DGRAM,0);
}

void reply_to_sender(int sock,char *msg,struct sockaddr_in *addrp,socklen_t len){
	char reply[BUFSIZ + BUFSIZ];
	sprintf(reply,"Thanks for your %d char message\n",strlen(msg));
	sendto(sock,reply,strlen(reply),0,(struct sockaddr *)addrp,len);
}

void recv_from_recv(int sock,struct sockaddr_in *addrp,socklen_t len){
	char buf[BUFSIZ];
	int msglen;

	if((msglen = recvfrom(sock,buf,BUFSIZ,0,(struct sockaddr *)addrp,&len) <= 0)){
		exit(1);
	}
	printf("receive msg %s\n",buf);
}
