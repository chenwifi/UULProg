#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include "dgram.h"
#include "lserv_funcs1.h"

#define SERVER_PORTNUM 13000
#define MAXUSERS 3
#define MSGLEN 128
#define RECLAIM_INTERVAL 5
#define TICKET_AVAIL 0
#define oops(m) {perror(m);exit(1);}

static int sd = -1;
static int ticket_array[MAXUSERS];
static int num_tickets_out = 0;

int setup(){
	sd = make_dgram_server_socket(SERVER_PORTNUM);
	if(sd == -1){
		oops("make socket");
	}

	free_all_tickets();

	return sd;
}

void free_all_tickets(){
	int i;

	for(i = 0;i < MAXUSERS; i++){
		ticket_array[i] = TICKET_AVAIL;
	}
}

void narrate(char *msg1,char *msg2,struct sockaddr_in *clientp){
	fprintf(stderr,"\t\tSERVER: %s %s",msg1,msg2);
	if(clientp){
		fprintf(stderr,"(%s:%d)",inet_ntoa(clientp->sin_addr),ntohs(clientp->sin_port));
		putc('\n',stderr);
	}
}

void do_request(char *req,struct sockaddr_in *clientp,socklen_t addlen){
	char *response;
	int ret;

	if(strncmp(req,"HELO",4) == 0){
		response = do_hello(req);
	}else if(strncmp(req,"GBYE",4) == 0){
		response = do_goodbye(req);
	}else{
		response = "FAIL invalid request";
	}

	narrate("SAID:",response,clientp);
	ret = sendto(sd,response,strlen(response),0,(struct sockaddr *)clientp,addlen);
	if(ret == -1){
		perror("SERVER sendto failed");
	}
}

char *do_hello(char *req){
	int x;
	static char replybuf[MSGLEN];

	if(num_tickets_out >= MAXUSERS){
		return "FAIL no tickets available";
	}

	for(x = 0;x < MAXUSERS && ticket_array[x] != TICKET_AVAIL;x++){
		;
	}

	if(x == MAXUSERS){
		narrate("database corrupt","",NULL);
		return "FAIL database corrupt";
	}

	ticket_array[x] = atoi(req + 5);
	sprintf(replybuf,"TICK %d.%d",ticket_array[x],x);
	num_tickets_out++;
	return replybuf;
}

char *do_goodbye(char *req){
	int pid,index;

	if(sscanf(req + 5,"%d.%d",&pid,&index) != 2 || (ticket_array[index] != pid)){
		narrate("Bogus ticket",req+5,NULL);
		return "FAIL invalid ticket";
	}

	ticket_array[index] = TICKET_AVAIL;
	num_tickets_out--;
	return "THNX See ya";
}

void ticket_reclaim(){
	int i;
	char tick[BUFSIZ];

	for(i = 0;i < MAXUSERS;i++){
		if((ticket_array[i] != TICKET_AVAIL) && (kill(ticket_array[i],0)) == -1 && (errno == ESRCH)){
			sprintf(tick,"%d.%d",ticket_array[i],i);
			narrate("freeing",tick,NULL);
			ticket_array[i] = TICKET_AVAIL;
			num_tickets_out--;
		}
	}

	alarm(RECLAIM_INTERVAL);
}
