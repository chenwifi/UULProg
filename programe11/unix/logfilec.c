#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/socket.h>

#define SOCKNAME "bigfatlog"
#define CNAME "dapanglog"
#define oops(m) {perror(m),exit(1);}

int main(){
	int sock;
	struct sockaddr_un addr;
	socklen_t len = sizeof(addr);
	char wbuf[32] = "i am bigfat";
	char rbuf[32];

	if((sock = socket(AF_UNIX,SOCK_DGRAM,0)) < 0){
		oops("socket");
	}

	unlink(CNAME);
	memset(&addr,0,sizeof(addr));
	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path,CNAME);
	if(bind(sock,(struct sockaddr *)&addr,len) < 0){
		oops("bind");
	}

	memset(&addr,0,sizeof(addr));
	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path,SOCKNAME);
	sendto(sock,wbuf,strlen(wbuf) + 1,0,(struct sockaddr *)&addr,len);


	recvfrom(sock,rbuf,32,0,(struct sockaddr *)&addr,&len);
	printf("receive from ser : %s\n",rbuf);
	unlink(CNAME);
	return 0;
}
