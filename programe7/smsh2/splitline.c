#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "smsh.h"

#define is_delim(x) ((x) == ' ' || (x) == '\t')

char *next_cmd(char *prompt,FILE *fp){
	int c;
	int pos = 0;
	int bufspace = 0;
	char *buf;

	printf("%s",prompt);
	while((c = getc(fp)) != EOF){
		if(pos + 1 >= bufspace){
			if(bufspace == 0){
				buf = (char *)emalloc(BUFSIZ);
			}else{
				buf = (char *)erealloc(buf,BUFSIZ + bufspace);
			}
			bufspace += BUFSIZ;
		}

		if(c == '\n'){
			break;
		}

		buf[pos++] = c;
	}

	if(c == EOF && pos == 0){
		return NULL;
	}

	buf[pos] = '\0';
	return buf;
}

char **splitline(char *line){
	char **args;
	int spots;
	char *start;
	char *c = line;
	int len,bufspace;
	int num = 0;

	if(line == NULL){
		return NULL;
	}

	args = (char**)emalloc(BUFSIZ);
	spots = BUFSIZ / sizeof(char *);
	bufspace = BUFSIZ;

	while(*c != '\0'){
		while(is_delim(*c))
			c++;

		if(*c == '\0'){
			break;
		}

		if(num + 1 >= spots){
			args = (char **)erealloc(args,bufspace+BUFSIZ);
			bufspace += BUFSIZ;
			spots = bufspace / sizeof(char *);
		}

		len = 1;
		start = c;
		while(*++c != '\0' && !is_delim(*c)){
			len++;
		}
		args[num++] = newstr(start,len);

	}
	args[num] = NULL;

	return args;
}

char *newstr(char *start,int len){
	char *rv = (char *)emalloc(len + 1);
	strncpy(rv,start,len);
	rv[len] = '\0';
	return rv;
}

void *emalloc(int size){
	void *rv;
	if((rv = malloc(size)) == NULL){
		fatal("out of memory","",1);
	}

	return rv;
}

void *erealloc(void *p,int size){
	void *rv;
	if((rv = realloc(p,size)) == NULL){
		fatal("realloc error","",1);
	}

	return rv;
}

void freelist(char **list){
	char **cp = list;
	while(*cp){
		free(*cp++);
	}
	free(list);
}
