#include <stdio.h>

int a[10][10];

struct test{
	int boolen;
	char *str;
};

struct test b[10];

int main(){
	printf("%d\n",a[5][5]);
	printf("%d\n",(b[5].str == NULL));

	return 0;
}
