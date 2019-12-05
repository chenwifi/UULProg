#include <stdio.h>
#include <errno.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

extern int server_requests;
extern int server_bytes_sent;
extern time_t server_started;

int http_reply(int fd,FILE **fpp,int code,char *msg,char *type,char *content){
	FILE *fp = fdopen(fd,"w");
	int bytes = 0;

	if(fp != NULL){
		bytes += fprintf(fp,"HTTP/1.0 %d %s\r\n",code,msg);
		bytes += fprintf(fp,"Content-type: %s\r\n\r\n",type);
		if(content){
			bytes += fprintf(fp,"%s\r\n",content);
		}
		fflush(fp);
	}

	if(fpp){
		*fpp = fp;
	}else{
		fclose(fp);
	}

	return bytes;
}

void not_implemented(int fd){
	http_reply(fd,NULL,501,"Not Implemented","text/plain","That command is not implemented");
}

int built_in(char *arg,int fd){
	FILE *fp;

	if(strcmp(arg,"status") != 0){
		return 0;
	}

	http_reply(fd,&fp,200,"OK","text/plain",NULL);
	fprintf(fp,"Server started: %s\n",ctime(&server_started));
	fprintf(fp,"Total request: %d\n",server_requests);
	fprintf(fp,"Bytes sent out: %d\n",server_bytes_sent);
	fclose(fp);

	return 1;
}

int not_exist(char *f){
	struct stat *buf;

	return ((stat(f,buf) == -1) && (errno == ENOENT));
}

void do_404(char *f,int fd){
	http_reply(fd,NULL,404,"Not Found","text/plain","The item you seek is not found");
}

int isadir(char *f){
	struct stat *buf;

	return ((stat(f,buf) != -1) && S_ISDIR(buf->st_mode));
}

void do_ls(char *f,int fd){
	FILE *fp;
	struct dirent *direntp;
	DIR *dirptr;
	int bytes = 0;

	bytes = http_reply(fd,&fp,200,"OK","text/plain",NULL);
	bytes += fprintf(fp,"Listing of Directory %s\n",f);

	if((dirptr = opendir(f)) != NULL){
		while((direntp = readdir(dirptr)) != NULL){
			bytes += fprintf(fp,"%s\n",direntp->d_name);
		}
		closedir(dirptr);
	}
	fclose(fp);
	server_bytes_sent += bytes;
}

char *file_type(char *f){
	char *cp;

	if((cp = strrchr(f,'.')) != NULL){
		return cp + 1;
	}

	return NULL;
}

void do_cat(char *f,int fd){
	char *extension = file_type(f);
	int bytes = 0;
	FILE *fpfile,*fpsock;
	int c;
	char *type = "text/plain";

	if(strcmp(extension,"html")){
		type = "text/html";
	}

	fpsock = fdopen(fd,"w");
	fpfile = fopen(f,"r");

	if(fpsock != NULL && fpfile != NULL){
		bytes = http_reply(fd,&fpsock,200,"OK",type,NULL);
		while((c = fgetc(fpfile)) != EOF){
			putc(c,fpsock);
			bytes++;
		}

		fclose(fpsock);
		fclose(fpfile);
	}

	server_bytes_sent += bytes;
}
