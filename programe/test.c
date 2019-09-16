#include <stdio.h>
#include "stack.h"
#include "getnum.h"

int main(){
	int c;
	c = getachar();
	printf("get a rand num  %d\n",c);

	c = getnum();
	printf("get a rand num  %d\n",c);
	setachar(c);
	c = getachar();
	printf("get a rand num  %d\n",c);
	
	return 0;
}
