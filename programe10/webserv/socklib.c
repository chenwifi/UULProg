#include <stdio.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>

#define BACKLOG 1
#define HOSTLEN 256

int make_server_socket_q(int portnum,int backlog);
int make_server_socket(int portnum){
	return make_server_socket_q(portnum,BACKLOG);
}

int make_server_socket_q(int portnum,int backlog){
	int sock_id;
	struct sockaddr_in saddr;
	struct hostent *hp;
	char hostname[HOSTLEN];

	sock_id = socket(AF_INET,SOCK_STREAM,0);
	if(sock_id == -1){
		return -1;
	}

	bzero((void *)&saddr,sizeof(saddr));
	gethostname(hostname,HOSTLEN);
	hp = gethostbyname(hostname);
	bcopy((void *)hp->h_addr,(void *)&saddr.sin_addr,hp->h_length);
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(portnum);
	if(bind(sock_id,(struct sockaddr *)&saddr,sizeof(saddr)) != 0){
		return -1;
	}

	if(listen(sock_id,backlog) != 0){
		return -1;
	}

	return sock_id;
}

int connect_to_server(char *host,int portnum){
	int sock;
	struct sockaddr_in servadd;
	struct hostent *hp;

	sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock == -1){
		return -1;
	}

	bzero((void *)&servadd,sizeof(servadd));
	hp = gethostbyname(host);
	if(hp == NULL){
		return -1;
	}
	bcopy((void *)hp->h_addr,(void *)&servadd.sin_addr,hp->h_length);
	servadd.sin_port = htons(portnum);
	servadd.sin_family = AF_INET;
	if(connect(sock,(struct sockaddr *)&servadd,sizeof(servadd)) != 0){
		return -1;
	}

	return sock;
}
