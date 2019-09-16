#include <stdio.h>

int main(){
	int c;
	while((c = getchar()) != 'Q'){
		printf("%c---%d\n",c,c);
	}
	return 0;
}
