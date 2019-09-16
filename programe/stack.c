#include <stdio.h>
#include "stack.h"
#include "getnum.h"

int str[100];
int ptr = 0;

int getachar(void){
	if(ptr<=0){
		return getnum();
	}else{
		return str[--ptr];
	}
}

void setachar(int c){
	str[ptr++] = c;
}
