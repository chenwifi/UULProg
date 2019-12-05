#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <curses.h>

#define MESSAGE "hello"
#define BLANK "     "

struct tmove{
	int row;
	int col;
	int dir;
	int delay;
};

void *moving_msg(void *arg);

int main(){
	struct tmove tm1;
	pthread_t t1;
	int c,ndelay;

	initscr();
	crmode();
	noecho();
	clear();

	tm1.row = 10;
	tm1.col = 0;
	tm1.dir = 1;
	tm1.delay = 200;

	move(tm1.row,tm1.col);
	addstr(MESSAGE);
	move(0,0);

	if(pthread_create(&t1,NULL,moving_msg,(void *)&tm1)){
		fprintf(stderr,"error creating thread");
		endwin();
		exit(0);
	}

	while(1){
		ndelay = 0;
		c = getch();
		if(c == 'Q'){
			break;
		}else if(c == ' '){
			tm1.dir = -tm1.dir;
		}else if(c == 'f' && tm1.delay > 2){
			ndelay = tm1.delay / 2;
		}else if(c == 's'){
			ndelay = tm1.delay * 2;
		}

		if(ndelay > 0){
			tm1.delay = ndelay;
		}
	}

	pthread_cancel(t1);
	endwin();

	return 0;
}

void *moving_msg(void *arg){
	struct tmove *tmp = (struct tmove *)arg;
	while(1){
		usleep(tmp->delay * 1000);
		if(tmp->dir == -1 && tmp->col <= 0){
			tmp->dir = 1;
		}else if(tmp->dir == 1 && tmp->col + strlen(MESSAGE) >= COLS){
			tmp->dir = -1;
		}
		move(tmp->row,tmp->col);
		addstr(BLANK);
		tmp->col += tmp->dir;
		move(tmp->row,tmp->col);
		addstr(MESSAGE);
		move(0,0);
		refresh();
	}
}
