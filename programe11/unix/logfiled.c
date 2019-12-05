#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/socket.h>

#define SOCKNAME "bigfatlog"
#define oops(m) {perror(m),exit(1);}

int main(){
	int sock;
	struct sockaddr_un addr;
	socklen_t addrlen;
	struct sockaddr_un clientun;
	socklen_t clientlen = sizeof(clientun);
	char rbuf[BUFSIZ];
	char wbuf[32] = "i am server";

	if((sock = socket(AF_UNIX,SOCK_DGRAM,0)) < 0){
		oops("socket");
	}

	unlink(SOCKNAME);
	memset(&addr,0,sizeof(addr));
	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path,SOCKNAME);
	addrlen = sizeof(addr);

	if(bind(sock,(struct sockaddr *)&addr,addrlen) < 0){
		oops("bind");
	}

	recvfrom(sock,rbuf,BUFSIZ,0,(struct sockaddr *)&clientun,&clientlen);

	printf("client addr : %s\n",clientun.sun_path);
	printf("receive msg : %s\n",rbuf);
	sendto(sock,wbuf,strlen(wbuf) + 1,0,(struct sockaddr *)&clientun,clientlen);
	unlink(SOCKNAME);

	return 0;
}
