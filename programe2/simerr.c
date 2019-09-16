#include <stdio.h>
#include <errno.h>
#include <fcntl.h>

int main(){
	int fd;
	fd = open("bigfat.txt",O_RDONLY);
	if(fd == -1){
		perror("open fail: ");
		return 1;
	}

	close(fd);
	return 0;
}
