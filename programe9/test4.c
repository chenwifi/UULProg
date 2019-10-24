#include <stdio.h>
#include <arpa/inet.h>

int main(){
	unsigned long a = 0x12345678;
	unsigned char *p = (unsigned char *)&a;

	printf("%d\n",sizeof(long));
	printf("%0x-%0x-%0x-%0x\n",p[0],p[1],p[2],p[3]);

	return 0;
}
