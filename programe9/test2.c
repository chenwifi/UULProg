#include <stdio.h>
#include <unistd.h>

int main(){
	pid_t pid;

	if((pid = fork()) == -1){
		perror("fork");
	}

	if(pid == 0){
		while(1)
			;
	}else{
		printf("child id %d\n",pid);
	}

	return 0;
}
