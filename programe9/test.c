#include <stdio.h>
#include <netdb.h>
#include <unistd.h>

#define HOSTLEN 256

int main(){
	char hostname[HOSTLEN];
	struct hostent *p;
	
	gethostname(hostname,HOSTLEN);
	printf("%s\n",hostname);

	printf("%d\n",htons(13000));

	//p = gethostbyname(hostname);
	//write(1,p->h_addr,p->h_length);

	return 0;
}
