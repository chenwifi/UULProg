#include <stdio.h>
#include "smsh.h"

int process(char **arglist){
	int rv;

	if(arglist[0] == NULL){
		rv = 0;
	}else if(is_control_command(arglist[0])){
		rv = do_control_command(arglist);
	}else if(ok_to_execute()){
		rv = execute(arglist);
	}

	return rv;
}
