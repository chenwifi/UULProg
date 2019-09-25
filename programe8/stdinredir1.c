#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int main(){
	int fd;
	int newfd;
	char line[100];

	fgets(line,100,stdin);
	printf("%s",line);
	fgets(line,100,stdin);
	printf("%s",line);
	fgets(line,100,stdin);
	printf("%s",line);


	fd = open("xyz",O_RDONLY);
	close(0);
	newfd = dup(fd);
	if(newfd != 0){
		fprintf(stderr,"could not open file fd 0\n");
		exit(1);
	}
	close(fd);
	fgets(line,100,stdin);
	printf("%s\n",line);
	fgets(line,100,stdin);
	printf("%s\n",line);
	fgets(line,100,stdin);
	printf("%s\n",line);

	return 0;
}
