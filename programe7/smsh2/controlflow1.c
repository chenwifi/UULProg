#include <stdio.h>
#include <string.h>
#include "smsh.h"

enum states if_state = NEUTRAL;
enum results if_result = SUCCESS;
int last_stat = 0;

int ok_to_execute(){
	int rv = 1;

	/*
	if(if_state == WANT_THEN){
		syn_err("then excepted");
		rv = 0;
	}
	*/

	if(if_state == THEN_BLOCK){
		if(if_result == FAIL){
			rv = 0;
		}
	}

	if(if_state == ELSE_BLOCK){
		if(if_result == SUCCESS){
			rv = 0;
		}
	}

	return rv;
}

int is_control_command(char *s){
	return (strcmp(s,"if") == 0 || strcmp(s,"then") == 0 || strcmp(s,"fi") == 0 || strcmp(s,"else") == 0);
}

int do_control_command(char **arglist){
	char *cmd = arglist[0];
	int rv = -1;

	if(strcmp(cmd,"if") == 0){
		if(if_state != NEUTRAL){
			rv = syn_err("if unexcepted");
		}else{
			last_stat = process(arglist + 1);
			if_result = last_stat == 0 ? SUCCESS : FAIL;
			if_state = WANT_THEN;
			rv = 0;
		}
	}else if(strcmp(cmd,"then") == 0){
		if(if_state != WANT_THEN){
			rv = syn_err("then unexcepted");
		}else{
			if_state = THEN_BLOCK;
			rv = 0;
		}
	}else if(strcmp(cmd,"fi") == 0){
		if(if_state == THEN_BLOCK || if_state == ELSE_BLOCK){
			if_state = NEUTRAL;
			rv = 0;
		}else{
			rv = syn_err("fi unexcepted");
		}
	}else if(strcmp(cmd,"else") == 0){
		if(if_state != THEN_BLOCK){
			rv = syn_err("else unexcepted");
		}else{
			if_state = ELSE_BLOCK;
			rv = 0;
		}
	}else{
		fatal("internal error processing; ",cmd,2);
	}
}

int syn_err(char *msg){
	if_state = NEUTRAL;
	fprintf(stderr,"syntax error: %s\n",msg);
	return -1;
}
