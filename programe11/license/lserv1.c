#include <stdio.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <errno.h>
#include "lserv_funcs1.h"

#define MSGLEN 128
#define RECLAIM_INTERVAL 5

int main(int argc,char *argv[]){
	int sock;
	int ret;
	char buf[MSGLEN];
	struct sockaddr_in client_addr;
	socklen_t addrlen = sizeof(client_addr);
	unsigned time_left;

	sock = setup();
	signal(SIGALRM,ticket_reclaim);
	alarm(RECLAIM_INTERVAL);

	while(1){
		ret = recvfrom(sock,buf,MSGLEN,0,(struct sockaddr *)&client_addr,&addrlen);
		if(ret != -1){
			buf[ret] = '\0';
			narrate("GOT:",buf,&client_addr);
			time_left = alarm(0);
			do_request(buf,&client_addr,addrlen);
			alarm(time_left);
		}else if(errno != EINTR){
			perror("recvfrom");
		}
	}

	return 0;
}
