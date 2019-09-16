#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#define DELAY 2

void child_code(int);
void parent_code(int);

int main(){
	int newpid;
	printf("before: mypid is %d\n",getpid());

	if((newpid = fork()) == -1){
		perror("fork");
		exit(1);
	}

	if(newpid == 0){
		child_code(DELAY);
	}else{
		parent_code(newpid);
	}
}

void child_code(int delay){
	printf("from child-%d\n",delay);
	sleep(delay);
	printf("sleep over\n");
}

void parent_code(int childpid){
	int ret;
	ret = wait(NULL);
	printf("from parent %d-%d\n",childpid,ret);
}
