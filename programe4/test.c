#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(){
	int original_flag,temp_flag;
	int c;
	
	original_flag = temp_flag =  fcntl(0,F_GETFL);
	temp_flag |= O_NONBLOCK;
	fcntl(0,F_SETFL,temp_flag);

	printf("can this show??");
	fflush(stdout);
	//sleep(3);
	//while((c = getchar()) != EOF && strchr("yYnN",c)==NULL){
	//}
	
	if((c = getchar()) != EOF){
		printf("inif\n");
	}

	printf("%d\n",c);

	fcntl(0,F_SETFL,original_flag);

	return 0;
}
