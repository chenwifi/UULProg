#include <stdio.h>
#include <curses.h>

int main(){
	initscr();

	clear();
	move(30,20);
	addstr("Hello,wwwwwwworld");
	move(LINES-1,0);
	//refresh();
	getch();
	sleep(5);

	endwin();

	return 0;
}
