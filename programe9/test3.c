#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(){
	FILE *fp;

	fp = popen("bc","w");
	//sleep(5);
	printf("start\n");
	fprintf(fp,"2+3\n");
	wait(NULL);
	pclose(fp);
	//sleep(10);

	return 0;
}
