#include <stdio.h>
#include <fcntl.h>

int main(){
	int fd;
	fd = open("xyz",O_RDONLY);
	printf("cdfd:%d\n",fd);

	return 0;
}
