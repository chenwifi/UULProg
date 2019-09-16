#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(){
	printf("my pid is %d\n",getpid());

	fork();
	fork();
	fork();
	printf("my pid is %d\n",getpid());
	sleep(1);

	return 0;
}
