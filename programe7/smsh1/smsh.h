#include <stdio.h>

char *next_cmd(char *,FILE *);
void *erealloc(void*,int);
void *emalloc(int);
char *newstr(char *,int);
char **splitline(char *);
int execute(char *arglist[]);
void setup();
