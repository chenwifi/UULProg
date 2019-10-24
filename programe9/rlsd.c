#include <stdio.h>
#include <ctype.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>

#define oops(msg) {perror(msg);exit(1);}
#define PORTNUM 13000
#define HOSTLEN 256

void sanitize(char *str);

int main(){
	int sock_id,sock_fd;
	struct sockaddr_in saddr;
	char hostname[HOSTLEN];
	struct hostent *hp;
	FILE *sock_fpi,*sock_fpo;
	FILE *pipe_fp;
	char dirname[BUFSIZ];
	char command[BUFSIZ];
	int c;

	sock_id = socket(AF_INET,SOCK_STREAM,0);
	if(sock_id == -1){
		oops("socket");
	}

	bzero((void *)&saddr,sizeof(saddr));
	gethostname(hostname,HOSTLEN);
	hp = gethostbyname(hostname);
	bcopy((void *)hp->h_addr,(void *)&saddr.sin_addr,hp->h_length);
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(PORTNUM);
	if(bind(sock_id,(struct sockaddr *)&saddr,sizeof(saddr)) == -1){
		oops("bind");
	}

	if(listen(sock_id,1) == -1){
		oops("listen");
	}

	while(1){
		sock_fd = accept(sock_id,NULL,NULL);	
		if(sock_fd == -1){
			oops("accpet");
		}

		if((sock_fpi = fdopen(sock_fd,"r")) == NULL){
			oops("fdopen");
		}

		if(fgets(dirname,BUFSIZ-5,sock_fpi) == NULL){
			oops("fgets");
		}

		//printf("%s",dirname);

		if((sock_fpo = fdopen(sock_fd,"w")) == NULL){
			oops("fdopen");
		}
		sprintf(command,"ls %s",dirname);
		sanitize(command);

		if((pipe_fp = popen(command,"r")) == NULL){
			oops("popen");
		}

		while((c = getc(pipe_fp)) != EOF){
			putc(c,sock_fpo);
		}

		pclose(pipe_fp);
		fclose(sock_fpo);
		fclose(sock_fpi);
	};

	return 0;
}

void sanitize(char *str){
	char *src,*dst;

	for(src = dst = str; *src ; src++){
		if(*src == '/' || isalnum(*src) || *src == ' '){
			*dst++ = *src;
		}
	}
	*dst = '\0';
}
