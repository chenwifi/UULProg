#include <stdio.h>
#include <signal.h>

int main(){
	int i;
	void f(int);
	signal(SIGINT,f);

	for(i = 0;i<5;i++){
		printf("hello\n");
		sleep(1);
	}

	return 0;
}

void f(int signum){
	printf("OUCH\n");
}
