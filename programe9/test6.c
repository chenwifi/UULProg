#include <stdio.h>
#include <unistd.h>

int main(){
	printf("%s\n",ttyname(0));

	return 0;
}
