#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

void do_ls(char *);
int main(int argc,char *argv[]){
	if(argc == 1){
		do_ls(".");
	}else{
		while(--argc > 0){
			printf("%s: \n",*++argv);
			do_ls(*argv);
		}
	}

	return 0;
}

void do_ls(char *filename){
	DIR *dir_ptr;
	struct dirent *direntp;
	struct stat info;

	if((dir_ptr = opendir(filename)) == NULL){
		fprintf(stderr,"ls1: cant open dir %s\n",filename);
	}else{
		while((direntp = readdir(dir_ptr)) != NULL){
			if((strcmp(direntp->d_name,".") == 0) || (strcmp(direntp->d_name,"..") == 0) )
				continue;
			printf("%s\n",direntp->d_name);
			stat(direntp->d_name,&info);
			printf("%o\n",info.st_mode);
			printf("%d\n",info.st_mtime);
		}
		closedir(dir_ptr);
	}
}
