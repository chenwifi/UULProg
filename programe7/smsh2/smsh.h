#include <stdio.h>

char *next_cmd(char *,FILE *);
void *erealloc(void*,int);
void *emalloc(int);
char *newstr(char *,int);
char **splitline(char *);
int execute(char *arglist[]);
void setup();
int process(char **);
int ok_to_execute();
int is_control_command(char *);
int do_control_command(char **);
int syn_err(char *);
void fatal(char *,char *,int);

enum states {NEUTRAL,WANT_THEN,THEN_BLOCK,ELSE_BLOCK};
enum results {SUCCESS,FAIL};
