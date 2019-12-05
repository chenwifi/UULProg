#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

#define oops(m,x) {perror(m);exit(x);}

void recv_from_recv(int sock,struct sockaddr_in *addrp,socklen_t len);

int main(int argc,char *argv[]){
	int sock;
	struct sockaddr_in saddr;
	char *msg;

	if(argc != 4){
		fprintf(stderr,"usage: host port message\n");
		exit(1);
	}

	if((sock = make_dgram_client_socket()) == -1){
		oops("cannot make socket",1);
	}

	make_internet_address(argv[1],atoi(argv[2]),&saddr);

	msg = argv[3];
	if(sendto(sock,msg,strlen(msg),0,(struct sockaddr *)&saddr,sizeof(saddr)) == -1){
		oops("sendto fail",1);
	}
	recv_from_recv(sock,&saddr,sizeof(saddr));

	return 0;
}
