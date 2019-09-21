#include <stdio.h>
#include "smsh.h"

extern enum states if_state;
extern enum results if_result;
extern int last_stat;

int process(char **arglist){
	int rv;

	if(arglist[0] == NULL){
		rv = 0;
	}else if(is_control_command(arglist[0])){
		rv = do_control_command(arglist);
	}else if(ok_to_execute()){
		if(!builtin_command(arglist,&rv)){
			rv = execute(arglist);
			if(if_state == WANT_THEN){
				last_stat = rv;
				if_result = (last_stat == 0) ? SUCCESS : FAIL;
			}
		}
	}

	return rv;
}
