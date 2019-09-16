#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

int main(){
	struct termios tmp;
	if(tcgetattr(0,&tmp) == -1){
		perror("tcgetattr");
		exit(1);
	}

	if(tmp.c_lflag & ECHO){
		printf("echo is on\n");
	}else{
		printf("echo is off\n");
	}

	return 0;
}
