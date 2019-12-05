int http_reply(int fd,FILE **fpp,int code,char *msg,char *type,char *content);
void not_implemented(int fd);
int not_exist(char *f);
void do_404(char *f,int fd);
int isadir(char *f);
void do_ls(char *f,int fd);
char *file_type(char *f);
void do_cat(char *f,int fd);
