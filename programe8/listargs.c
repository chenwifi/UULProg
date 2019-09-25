#include <stdio.h>

int main(int argc,char *argv[]){
	int i;
	printf("Number of args is %d\n",argc);

	for(i = 0;i<argc;i++){
		printf("args[%d] is %s\n",i,argv[i]);
	}
	fprintf(stderr,"This is sent to stderr\n");

	return 0;
}
