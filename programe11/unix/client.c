#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>

int main(){
	int fd;
	struct sockaddr_un un;
	char path[32] = "clientbigfile";
	char serverpath[32] = "bigfatfile";
	socklen_t addr_len = sizeof(un);
	char rbuf[32];
	char wbuf[32] = "i am client";

	if((fd = socket(AF_UNIX,SOCK_STREAM,0)) < 0){
		perror("socket");
		return -1;
	}

	memset(&un,0,sizeof(un));
	un.sun_family = AF_UNIX;
	strncpy(un.sun_path,path,32);
	unlink(path);

	/*
	if(bind(fd,(struct sockaddr *)&un,addr_len) < 0){
		perror("bind");
		return -1;
	}
	*/

	memset(&un,0,sizeof(un));
	un.sun_family = AF_UNIX;
	strncpy(un.sun_path,serverpath,32);
	if(connect(fd,(struct sockaddr *)&un,addr_len) < 0){
		perror("connect");
		return -1;
	}

	if(write(fd,wbuf,strlen(wbuf) + 1) < 0){
		perror("write");
		return -1;
	}
	if(read(fd,rbuf,32) < 0){
		perror("read");
		return -1;
	}
	printf("receive msg:%s\n",rbuf);
	unlink(path);

	return 0;
}
