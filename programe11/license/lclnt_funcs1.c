#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <strings.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "dgram.h"
#include "lclnt_funcs1.h"

#define HOSTLEN 512
#define SERVER_PORTNUM 13000
#define MSGLEN 128

#define oops(p) {perror(p);exit(1);}

static int pid = -1;
static int sd = -1;
static struct sockaddr_in serv_addr;
static socklen_t serv_alen;
static have_ticket = 0;
static char ticket_buf[128];

void setup(){
	char hostname[HOSTLEN];

	pid = getpid();
	sd = make_dgram_client_socket();
	if(sd == -1){
		oops("Cannot create socket");
	}
	gethostname(hostname,HOSTLEN);//server on same host
	make_internet_address(hostname,SERVER_PORTNUM,&serv_addr);
	serv_alen = sizeof(serv_addr);
}

int get_ticket(){
	char *response;
	char buf[MSGLEN];

	if(have_ticket){
		return 0;
	}

	sprintf(buf,"HELO %d",pid);
	if((response = do_transaction(buf)) == NULL){
		return -1;
	}

	if(strncmp(response,"TICK",4) == 0){
		strcpy(ticket_buf,response + 5);
		have_ticket = 1;
		narrate("got ticket",ticket_buf);
		return 0;
	}

	if(strncmp(response,"FAIL",4) == 0){
		narrate("Could not get ticket",response);
	}else{
		narrate("Unknown message:",response);
	}

	return -1;
}

char *do_transaction(char *msg){
	int ret;
	static char buf[MSGLEN];
	struct sockaddr_in retaddr;
	socklen_t addrlen = sizeof(retaddr);

	ret = sendto(sd,msg,strlen(msg),0,(struct sockaddr *)&serv_addr,serv_alen);
	if(ret == -1){
		syserr("sendto");
		return NULL;
	}

	ret = recvfrom(sd,buf,MSGLEN,0,(struct sockaddr *)&retaddr,&addrlen);
	if(ret == -1){
		syserr("recvfrom");
		return NULL;
	}

	return buf;
}

void narrate(char *msg1,char *msg2){
	fprintf(stderr,"CLIENT[%d]: %s %s\n",pid,msg1,msg2);
}

void syserr(char *msg){
	char buf[MSGLEN];
	sprintf(buf,"CLIENT[%d]:%s",pid,msg);
	perror(buf);
}

void shut_down(){
	close(sd);
}

int release_ticket(){
	char *response;
	char buf[MSGLEN];

	if(!have_ticket){
		return 0;
	}

	sprintf(buf,"GBYE %s",ticket_buf);
	if((response = do_transaction(buf)) == NULL){
		return -1;
	}

	if(strncmp(response,"THNX",4) == 0){
		narrate("release ticket ok","");
		return 0;
	}

	if(strncmp(response,"FAIL",4) == 0){
		narrate("release fail",response + 5);
	}else{
		narrate("Unknown message:",response);
	}

	return -1;
}
