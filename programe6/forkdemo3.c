#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(){
	int fork_rv;
	printf("my pid is %d\n",getpid());

	fork_rv = fork();
	if(fork_rv == -1){
		perror("fork");
	}else if(fork_rv == 0){
		printf("i am child,my pid is %d\n",getpid());
	}else{
		printf("i am parent,my child pid is %d\n",fork_rv);
	}
	sleep(1);

	return 0;
}
