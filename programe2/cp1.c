#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#define COPYMODE 0644
#define BUFFERSIZE 4096

void oops(char *,char *);
int main(int argc,char *argv[]){
	int fd1,fd2,count;
	char buf[BUFFERSIZE];

	if(argc != 3){
		fprintf(stderr,"usage: %s source destination",*argv);
		exit(1);
	}

	if((fd1 = open(argv[1],O_RDONLY)) == -1){
		oops("cant open ",argv[1]);
	}
	if((fd2 = creat(argv[2],COPYMODE)) == -1){
		oops("cant creat ",argv[2]);
	}

	while((count = read(fd1,buf,BUFFERSIZE)) > 0){
		if(write(fd2,buf,count) != count){
			oops("write error ",argv[2]);
		}
	}
	if(count == -1){
		oops("read error ",argv[1]);
	}

	if(close(fd1) == -1 || close(fd2) == -1){
		oops("error close file","");
	}
	
	return 0;
}

void oops(char *s,char *c){
	fprintf(stderr,"Error: %s",s);
	perror(c);
	exit(1);
}
