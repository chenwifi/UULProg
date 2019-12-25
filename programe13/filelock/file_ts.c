#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//#define oops (msg,x) {perror(msg);exit(x);}
#define oops(msg,x) {perror(msg);exit(x);}

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
	int fd;
	time_t now;
	char *message;

	if(argc != 2){
		fprintf(stderr,"usage : file_ts filename\n");
		exit(1);
	}

	if((fd = open(argv[1],O_CREAT | O_WRONLY | O_TRUNC,0644)) == -1){
		oops(argv[1],1);
	}

	while(1){
		time(&now);
		message = ctime(&now);
		lock_operation(fd,F_WRLCK);

		if(lseek(fd,0,SEEK_SET) == -1){
			printf("%d\n",errno);
			if(errno == EOVERFLOW){
				fprintf(stderr,"haha");
			}
			oops("lseek",1);
		}

		if(write(fd,message,strlen(message)) == 1){
			oops("write",1);
		}
		lock_operation(fd,F_UNLCK);
		sleep(1);
	}

	return 0;
}
