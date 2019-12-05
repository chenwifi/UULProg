#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>

int main(){
	pid_t pid;

	if((pid = fork()) == -1){
		perror("fork");
		exit(1);
	}

	return 0;
}
