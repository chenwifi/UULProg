#include <stdio.h>

int main(){
	char cmd[BUFSIZ],arg[BUFSIZ];
	char msg[BUFSIZ] = "get hello http/1.0";

	sscanf(msg,"%s%s",cmd,arg);
	printf("%s--%s\n",cmd,arg);
}
