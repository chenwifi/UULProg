#include <stdio.h>

int main(){
	int pid,index;
	char *res = "GBYE 1234.2";

	printf("%s\n",res + 5);
	if(sscanf(res + 5,"%d.%d",&pid,&index) != 2){
	}else{
		printf("pid:%d,index:%d\n",pid,index);
	}

	return 0;
}
