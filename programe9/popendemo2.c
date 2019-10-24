#include <stdio.h>

int main(){
	FILE *fp;

	fp = popen("bc","w");
	fprintf(fp,"2+3\n");
	pclose(fp);

	return 0;
}
