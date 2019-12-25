#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define oops(msg,x) {perror(msg);exit(x);}
#define BUFLEN 20

void lock_operation(int fd,int op){
	struct flock lock;
	lock.l_type = op;
	lock.l_whence = SEEK_SET;
	lock.l_start = 0;
	lock.l_len = 0;

	if(fcntl(fd,F_SETLKW,&lock) == -1){
		oops("lock operation",1);
	}
}

int main(int argc,char *argv[]){
	int fd,nread;
	char buf[BUFLEN];

	if(argc != 2){
		fprintf(stderr,"usage : file_tc filename\n");
		exit(1);
	}

	if((fd = open(argv[1],O_RDONLY)) == -1){
		oops(argv[1],1);
	}

	lock_operation(fd,F_RDLCK);
	while((nread = read(fd,buf,BUFLEN)) > 0){
		write(1,buf,nread);
	}
	lock_operation(fd,F_UNLCK);
}
