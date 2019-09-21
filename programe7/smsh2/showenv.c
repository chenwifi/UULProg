#include <stdio.h>

extern char **environ;

int main(){
	printf("%s\n",environ[1]);

	return 0;
}
