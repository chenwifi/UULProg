#include <stdio.h>
#include <stdlib.h>
#include "socklib.c"

#define LEN 256
#define HOST "localhost"
#define PORTNUM 13000

void talk_with_server(int fd);
int main(){
	int sock;
	sock = connect_to_server(HOST,PORTNUM);
	if(sock == -1){
		exit(1);
	}
	talk_with_server(sock);
	close(sock);

	return 0;
}

void talk_with_server(int fd){
	char buf[LEN];
	int n;

	n = read(fd,buf,LEN);
	write(1,buf,n);
}
