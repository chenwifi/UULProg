#include <stdio.h>
#include <signal.h>

#define INPUTLEN 100

void inthandler(int);
void quithandler(int);

int main(int argc,char **argv){
	char input[INPUTLEN];
	int nchars;

	signal(SIGINT,inthandler);
	signal(SIGQUIT,quithandler);

	do{
		printf("\nType Message\n");
		nchars = read(0,input,INPUTLEN);

		if(nchars == -1){
			perror("read error");
		}else{
			input[nchars] = '\0';
			printf("you type %s\n",input);
		}
	}while(strncmp(input,"quit",4) != 0);

	return 0;
}

void inthandler(int signum){
	static int a = 1;
	printf("receive signum %d\n",signum);
	sleep(5);
	printf("leaving inthandler: %d\n",a++);
}
void quithandler(int signum){
	printf("receive signum %d\n",signum);
	sleep(2);
	printf("leaving quithandler");
}
