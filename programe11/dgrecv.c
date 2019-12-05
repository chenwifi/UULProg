#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>

#define oops(m,x) {perror(m);exit(x);}

int make_dgram_server_socket(int);
void say_who_called(struct sockaddr_in *);
int get_internet_address(char *host,int len,int *portp,struct sockaddr_in *addrp);
void reply_to_sender(int sock,char *msg,struct sockaddr_in *addrp,socklen_t len);

int main(int argc,char *argv[]){
	int port;
	int sock;
	ssize_t msglen;
	char buf[BUFSIZ];
	struct sockaddr_in saddr;
	socklen_t saddrlen;

	if(argc != 2 || (port = atoi(argv[1])) <= 0){
		fprintf(stderr,"usage: dgrecv portnum\n");
		exit(1);
	}

	if((sock = make_dgram_server_socket(port)) == -1){
		oops("can not make socket",1);
	}
	saddrlen = sizeof(saddr);
	while((msglen = recvfrom(sock,buf,BUFSIZ,0,(struct sockaddr *)&saddr,&saddrlen)) > 0){
		buf[msglen] = '\0';
		printf("dgrecv: got a message: %s\n",buf);
		say_who_called(&saddr);
		reply_to_sender(sock,buf,&saddr,saddrlen);
	}

	return 0;
}

void say_who_called(struct sockaddr_in *addrp){
	char host[BUFSIZ];
	int port;

	get_internet_address(host,BUFSIZ,&port,addrp);
	printf("from %s:%d\n",host,port);
}
