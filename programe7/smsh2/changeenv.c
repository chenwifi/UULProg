#include <stdio.h>
#include <unistd.h>

extern char **environ;

int main(){
	char *table[3];
	table[0] = "name=bigfat";
	table[1] = "nickname=dapang";
	table[2] = NULL;

	environ = table;
	execlp("env","env",NULL);

	return 0;
}
