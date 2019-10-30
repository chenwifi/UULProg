#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

void header(FILE *fp,char *content_type){
	fprintf(fp,"HTTP/1.0 200 OK\r\n");

	if(content_type != NULL){
		fprintf(fp,"Content-type: %s\r\n",content_type);
	}
}

void header2(int fd,char *content_type){
	FILE *fp = fdopen(fd,"w");
	fprintf(fp,"HTTP/1.0 200 OK\r\n");

	if(content_type != NULL){
		fprintf(fp,"Content-type: %s\r\n",content_type);
	}
}

void cannot_do(int fd){
	FILE *fp = fdopen(fd,"w");
	fprintf(fp,"HTTP/1.0 501 Not Implemented\r\n");
	fprintf(fp,"Content-Type: text/plain\r\n");
	fprintf(fp,"\r\n");
	fprintf(fp,"That command is not yet implemented\r\n");
	fclose(fp);
}

void do_404(char *item,int fd){
	FILE *fp = fdopen(fd,"w");
	fprintf(fp,"HTTP/1.0 404 Not Found\r\n");
	fprintf(fp,"Content-Type: text/plain\r\n");
	fprintf(fp,"\r\n");
	fprintf(fp,"The item you requested %s is not found\r\n",item);
	fclose(fp);
}

int isadir(char *f){
	struct stat *buf;

	return ((stat(f,buf) != -1) && S_ISDIR(buf->st_mode));
}

int not_exist(char *f){
	struct stat *buf;

	return ((stat(f,buf) == -1) && (errno == ENOENT));
}

void do_ls(char *dir,int fd){
	FILE *fp;
	fp = fdopen(fd,"w");

	//header(fp,"text/plain");
	//header2(fd,"text/plain");
	fprintf(fp,"\r\n");
	fflush(fp);

	dup2(fd,1);
	dup2(fd,2);
	close(fd);
	execlp("ls","ls","-l",dir,NULL);
	perror("ls");
	exit(1);
}

char *file_type(char *f){
	char *cp;

	if((cp = strrchr(f,'.')) != NULL){
		return cp + 1;
	}
	return NULL;
}

int ends_in_cgi(char *f){
	char *cp;
	cp = file_type(f);
	return (strcmp(cp,"cgi") == 0);
}

void do_exec(char *prog,int fd){
	FILE *fp;
	fp = fdopen(fd,"w");
	
	header(fp,"NULL");
	dup2(fd,1);
	dup2(fd,2);
	close(fd);
	execl(prog,prog,NULL);
	perror(prog);
}

void do_cat(char *f,int fd){
	//char *extension = file_type(f);
	char *content;
	FILE *fpfile,*fpsock;
	int c;

	/*
	if(strcmp(extension,"html") == 0){
		content = "text/html";
	}else if(strcmp(extension,"jpg") == 0){
		content = "text/jpeg";
	}else if(strcmp(extension,"jpge") == 0){
		content = "text/jpeg";
	}else{
		content = "text/plain";
	}
	*/

	fpsock = fdopen(fd,"w");
	fpfile = fopen(f,"r");
	if(fpsock != NULL && fpfile != NULL){
		//header(fpsock,content);
		fprintf(fpsock,"\r\n");
		while((c = fgetc(fpfile)) != EOF){
			fputc(c,fpsock);
		}
		fclose(fpsock);
		fclose(fpfile);
		return ;
	}
}
