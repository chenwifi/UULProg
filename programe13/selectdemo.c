#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>

#define oops(m,x) {perror(m);exit(x);}

void showdata(char *filename,int fd){
	char buf[BUFSIZ];
	int n;

	printf("%s :",filename);
	fflush(stdout);
	n = read(fd,buf,BUFSIZ);
	if(n == -1){
		oops(filename,1);
	}
	write(1,buf,n);
	write(1,"\n",1);
}

int main(int argc,char *argv[]){
	int fd1,fd2;
	fd_set readfds;
	struct timeval timeout;
	int maxfd;
	int retval;

	if(argc != 4){
		fprintf(stderr,"usage: %s file file time",argv[0]);
		exit(1);
	}

	if((fd1 = open(argv[1],O_RDONLY)) == -1){
		oops(argv[1],1);
	}
	/*
	if((fd2 = open(argv[2],O_RDONLY)) == -1){
		oops(argv[2],1);
	}
	*/
	fd2 = 0;
	maxfd = 1 + (fd1 > fd2 ? fd1 : fd2);

	while(1){
		FD_ZERO(&readfds);
		FD_SET(fd1,&readfds);
		FD_SET(fd2,&readfds);

		timeout.tv_sec = atoi(argv[3]);
		timeout.tv_usec = 0;

		retval = select(maxfd,&readfds,NULL,NULL,&timeout);
		printf("retval is %d\n",retval);

		if(retval == -1){
			oops("select",1);
		}

		if(retval > 0){
			if(FD_ISSET(fd1,&readfds)){
				showdata(argv[1],fd1);
			}
			if(FD_ISSET(fd2,&readfds)){
				showdata(argv[2],fd2);
			}
		}else{
			printf("no input after %d seconds\n",atoi(argv[3]));
		}
	}

	return 0;
}
