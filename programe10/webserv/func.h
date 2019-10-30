#include <stdio.h>

void header(FILE *fp,char *content_type);
void cannot_do(int fd);
void do_404(char *item,int fd);
int isadir(char *f);
int not_exist(char *f);
void do_ls(char *dir,int fd);
char *file_type(char *f);
int ends_in_cgi(char *f);
void do_exec(char *prog,int fd);
void do_cat(char *f,int fd);
