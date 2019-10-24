#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>

#define oops(msg) {perror(msg);exit(1);}
#define PORTNUM 13000

int main(int argc,char **argv){
	int sock_id;
	struct sockaddr_in servadd;
	struct hostent *hp;
	char buffer[BUFSIZ];
	int n_read;

	if(argc != 3){
		exit(1);
	}

	sock_id = socket(AF_INET,SOCK_STREAM,0);
	if(sock_id == -1){
		oops("socket");
	}

	bzero((void *)&servadd,sizeof(servadd));
	hp = gethostbyname(argv[1]);
	bcopy((void *)hp->h_addr,(void *)&servadd,hp->h_length);
	servadd.sin_port = htons(PORTNUM);
	servadd.sin_family = AF_INET;
	if(connect(sock_id,(struct sockaddr *)&servadd,sizeof(servadd)) == -1){
		oops("connect");
	}

	if(write(sock_id,argv[2],strlen(argv[2])) == -1){
		oops("write");
	}
	if(write(sock_id,"\n",1) == -1){
		oops("write");
	}

	while((n_read = read(sock_id,buffer,BUFSIZ)) > 0 ){
		if(write(1,buffer,n_read) != n_read){
			oops("write");
		}
	}
	close(sock_id);


	return 0;
}
