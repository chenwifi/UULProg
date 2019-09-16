#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc,char **argv){
	int fd;
	char tmp[BUFSIZ];

	if(argc != 2){
		fprintf(stderr,"usage: write1 tty\n");
		exit(1);
	}

	if((fd = open(argv[1],O_WRONLY)) == -1){
		perror(argv[1]);
		exit(1);
	}

	while(fgets(tmp,BUFSIZ,stdin) != NULL){
		if(write(fd,tmp,strlen(tmp)) == -1){
			break;
		}
	}
	//write(fd,"hello",5);
	close(fd);

	return 0;
}
