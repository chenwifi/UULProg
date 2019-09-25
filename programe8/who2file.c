#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int main(){
	int pid,fd;
	
	pid = fork();

	if(pid == -1){
		perror("fork");
		exit(1);
	}else if(pid == 0){
		close(1);
		fd = creat("userlist",0644);
		execlp("who","who",NULL);
	}else{
		wait(NULL);
		printf("Done running\n");
	}

	return 0;
}
