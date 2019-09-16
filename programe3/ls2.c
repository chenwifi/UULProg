#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <stdlib.h>
#include <time.h>

void do_ls(char *);
void do_stat(char *);
void show_file_info(char *,struct stat *);
void mode_to_letters(int mode,char *str);
char *uid_to_name(uid_t uid);
char *gid_to_name(gid_t gid);

int main(int argc,char **argv){
	if(argc == 1){
		do_ls(".");
	}else{
		do_ls(*++argv);
	}

	return 0;
}

void do_ls(char *dirname){
	DIR *dirp;
	struct dirent *direntp;

	if((dirp = opendir(dirname)) == NULL){
		fprintf(stderr,"ls2: cant open dir%s",dirname);
	}else{
		while((direntp = readdir(dirp)) != NULL){
			do_stat(direntp->d_name);
		}
		closedir(dirp);
	}
}

void do_stat(char *filename){
	struct stat info;

	if(stat(filename,&info) == -1){
		perror(filename);
	}else{
		show_file_info(filename,&info);
	}
}

void show_file_info(char *filename,struct stat *info){
	char modestr[11];

	mode_to_letters(info->st_mode,modestr);
	printf("%s",modestr);
	printf("%4d",(int)info->st_nlink);
	printf("%-8s",uid_to_name(info->st_uid));
	printf("%-8s",gid_to_name(info->st_gid));
	printf("%8ld",(long)info->st_size);
	printf("%.12s",ctime(&info->st_mtime));
	printf("%s\n",filename);
}

void mode_to_letters(int mode,char *str){
	strcpy(str,"----------");

	if(S_ISDIR(mode)) str[0] = 'd';
	if(S_ISCHR(mode)) str[0] = 'c';
	if(S_ISBLK(mode)) str[0] = 'b';

	if(mode & S_IRUSR) str[1] = 'r';
	if(mode & S_IWUSR) str[2] = 'w';
	if(mode & S_IXUSR) str[3] = 'x';

	if(mode & S_IRGRP) str[4] = 'r';
	if(mode & S_IWGRP) str[5] = 'w';
	if(mode & S_IXGRP) str[6] = 'x';

	if(mode & S_IROTH) str[7] = 'r';
	if(mode & S_IWOTH) str[8] = 'w';
	if(mode & S_IXOTH) str[9] = 'x';

	str[10] = '\0';
}

char *uid_to_name(uid_t uid){
	struct passwd *user;
	char *tmp = (char *)malloc(10);

	if((user = getpwuid(uid)) == NULL){
		sprintf(tmp,"%d",uid);
		return tmp;
	}else{
		return user->pw_name;
	}
}

char *gid_to_name(gid_t gid){
	struct group *grp;
	char *tmp = (char *)malloc(10);

	if((grp = getgrgid(gid)) == NULL){
		sprintf(tmp,"%d",gid);
		return tmp;
	}else{
		return grp->gr_name;
	}
}
