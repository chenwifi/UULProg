#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>

ino_t get_inode(char *);
void printpathto(ino_t);
void inum_to_name(ino_t,char *,int);
int main(){
	printpathto(get_inode("."));
	printf("\n");

	return 0;
}

void printpathto(ino_t inum){
	char name[BUFSIZ];
	ino_t this_inum;

	if((this_inum = get_inode("..")) != inum){
		chdir("..");
		inum_to_name(inum,name,BUFSIZ);
		printpathto(this_inum);
		printf("/%s",name);
	}
}

void inum_to_name(ino_t inum,char *name,int size){
	DIR *dirp;
	struct dirent *direntp;

	if((dirp = opendir(".")) == NULL){
		fprintf(stderr,"Cannot opendir");
		perror(name);
		exit(1);
	}

	while((direntp = readdir(dirp)) != NULL){
		if(direntp->d_ino == inum){
			strncpy(name,direntp->d_name,size);
			name[size-1] = '\0';
			closedir(dirp);
			return ;
		}
	}

	fprintf(stderr,"Cannot find name");
	exit(1);
}

ino_t get_inode(char *dirname){
	struct stat tmp;
	if(stat(dirname,&tmp) == -1){
		fprintf(stderr,"Cannot stat");
		perror(dirname);
		exit(1);
	}

	return tmp.st_ino;
}
