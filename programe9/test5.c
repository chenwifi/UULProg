#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(){
	int pipefd[2];
	FILE *fp;
	char message[BUFSIZ];

	pipe(pipefd);
	fp = fdopen(pipefd[0],"r");
	write(pipefd[1],"hello\naa",sizeof("hello\naa"));
	fgets(message,BUFSIZ,fp);

	printf("%s",message);
	printf("%d\n",message[5]);

	return 0;
}
