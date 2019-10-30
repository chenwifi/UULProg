#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

int not_exist(char *f);

char *file_type(char *f);
int myabs(int);

int main(){
	char cmd[BUFSIZ],arg[BUFSIZ];
	char s[BUFSIZ];
	char *rq = "GET index.html HTTP/1.1";
	//int b = -3;
	//printf("%d\n",myabs(b));
	
	/*
	s[0] = 'r';
	s[1] = 'q';
	s[2] = '\0';
	sscanf(rq,"%s%s",cmd,arg);
	printf("%s\n",s);
	printf("%d\n",strlen(s));
	printf("%s\n",arg);
	*/

	//printf("%d\n",not_exist("bigfat"));

	//char arg[BUFSIZ];
	/*
	arg[0] = '.';
	arg[1] = '/';
	arg[2] = 'c';
	arg[3] = 'd';
	arg[4] = '.';
	arg[5] = 'h';
	*/
	strcpy(arg,"./");
	if(sscanf(rq,"%s%s",cmd,arg+2) != 2){
		return ;
	}
	//arg[6] = '\0';
	char *tmp = arg;
	char *cp = file_type(tmp);
	printf("%s\n",cp);

	return 0;
}
char *file_type(char *f){
	char *cp;

	if((cp = strrchr(f,'.')) != NULL){
		return cp + 1;
	}
	return NULL;
}
int not_exist(char *f){
	struct stat *buf;

	return ((stat(f,buf) == -1) && (errno == ENOENT));
}
