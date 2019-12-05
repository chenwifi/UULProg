#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

int main(){
	int fd;
	int clientfd;
	socklen_t client_addr_len = sizeof(struct sockaddr_un);
	socklen_t addr_len = sizeof(struct sockaddr_un);
	char path[32] = "bigfatfile";
	struct sockaddr_un un,clientun;
	char rbuf[32];
	char wbuf[32] = "i am server";

	if((fd = socket(AF_UNIX,SOCK_STREAM,0)) < 0){
		perror("socket");
		return -1;
	}

	unlink(path);

	memset(&un,0,sizeof(un));
	un.sun_family = AF_UNIX;
	strncpy(un.sun_path,path,32);

	if(bind(fd,(struct sockaddr *)&un,addr_len) < 0){
		perror("bind");
		return -1;
	}

	if(listen(fd,1) < 0){
		perror("listen");
		return -1;
	}

	if((clientfd = accept(fd,(struct sockaddr *)&clientun,&client_addr_len)) < 0){
		perror("accept");
		return -1;
	}

	printf("client is:%s\n",clientun.sun_path);

	if(read(clientfd,rbuf,32) < 0){
		perror("read");
		return -1;
	}
	printf("receive msg:%s\n",rbuf);

	if(write(clientfd,wbuf,strlen(wbuf) + 1) < 0){
		perror("write");
		return -1;
	}

	unlink(path);

	return 0;
}
