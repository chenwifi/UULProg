#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curses.h>
#include <pthread.h>

int main(){
	int i;

	initscr();
	crmode();
	noecho();
	clear();

	move(3,4);
	addch('a');
	for(i = 0;i<4;i++){
		addch(' ');
	}
	addch('f');
	refresh();
	sleep(10);
	endwin();

	return 0;
}
