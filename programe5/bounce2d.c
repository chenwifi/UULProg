#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>
#include <curses.h>
#include <string.h>
#include "bounce.h"

struct ppball the_ball;

int top_movewall;
int movewall_len;

void set_up();
void wrap_up();
void ball_move(int);
int bounce_or_lose(struct ppball *);
void print_wall();
void print_movewall();
void move_wall(int);
void free_movewall();
void move_wall_1(int signum);

int main(){
	int c;
	top_movewall = TOP_ROW;
	movewall_len = 3;

	set_up();
	while((c = getchar()) != 'q'){
		if(c == 'f'){
			the_ball.x_ttm--;
		}
		if(c == 's'){
			the_ball.x_ttm++;
		}
		if(c == 'F'){
			the_ball.y_ttm--;
		}
		if(c == 'S'){
			the_ball.y_ttm++;
		}
		if(c == 'j'){
			move_wall(c);
		}
		if(c == 'l'){
			move_wall(c);
		}
	}
	wrap_up();

	return 0;
}

void set_up(){
	the_ball.y_pos = Y_INIT;
	the_ball.x_pos = X_INIT;
	the_ball.y_ttg = the_ball.y_ttm = Y_TTM;
	the_ball.x_ttg = the_ball.x_ttm = X_TTM;
	the_ball.x_dir = the_ball.y_dir = 1;
	the_ball.symbol = DEF_SYMBOL;
	
	/*
	initscr();
	noecho();
	crmode();

	print_wall();
	print_movewall();
	*/

	//signal(SIGINT,SIG_IGN);
	//mvaddch(the_ball.x_pos,the_ball.y_pos,the_ball.symbol);
	//refresh();

	signal(SIGALRM,move_wall_1);
	set_ticker(1000/TICKS_PER_SEC);
}
void move_wall_1(int signum){
	//printf("haha\n");
	//sleep(2);
}

void print_wall(){
	int i;
	for(i = LEFT_EDGE;i<=RIGHT_EDGE;i++){
		mvaddch(TOP_ROW-1,i,'-');
	}
	for(i = TOP_ROW;i<=BOT_ROW;i++){
		mvaddch(i,LEFT_EDGE-1,'|');
	}
	for(i = LEFT_EDGE;i<=RIGHT_EDGE;i++){
		mvaddch(BOT_ROW+1,i,'-');
	}
}

void move_wall(int dir){
	free_movewall();
	if(dir == 'j'){
		if(top_movewall>TOP_ROW){
			top_movewall--;
		}
	}else if(dir == 'l'){
		if(top_movewall+movewall_len < BOT_ROW){
			top_movewall++;
		}
	}
	print_movewall();
	refresh();
}

void free_movewall(){
	int i;
	for(i = 0;i<movewall_len;i++){
		mvaddch(top_movewall+i,RIGHT_EDGE+1,BLANK);
	}
}

void wrap_up(){
	set_ticker(0);
	endwin();
}

void print_movewall(){
	int i;
	for(i = 0;i<movewall_len;i++){
		mvaddch(top_movewall+i,RIGHT_EDGE+1,'|');
	}
}

void ball_move(int signum){
	int y_cur,x_cur,moved;

	y_cur = the_ball.y_pos;
	x_cur = the_ball.x_pos;
	moved = 0;

	if(the_ball.y_ttm > 0 && the_ball.y_ttg-- == 1){
		the_ball.y_pos += the_ball.y_dir;
		the_ball.y_ttg = the_ball.y_ttm;
		moved = 1;
	}

	if(the_ball.x_ttm > 0 && the_ball.x_ttg-- == 1){
		the_ball.x_pos += the_ball.x_dir;
		the_ball.x_ttg = the_ball.x_ttm;
		moved = 1;
	}

	if(moved){
		mvaddch(y_cur,x_cur,BLANK);
		mvaddch(the_ball.y_pos,the_ball.x_pos,the_ball.symbol);
		move(LINES-1,COLS-1);
		bounce_or_lose(&the_ball);
		refresh();
	}
}

int bounce_or_lose(struct ppball *bp){
	int return_val = 0;

	if(bp->y_pos == TOP_ROW){
		bp->y_dir = 1;
		return_val = 1;
	}else if(bp->y_pos == BOT_ROW){
		bp->y_dir = -1;
		return_val = 1;
	}
	if(bp->x_pos == LEFT_EDGE){
		bp->x_dir = 1;
		return_val = 1;
	}else if(bp->x_pos == RIGHT_EDGE){
		if(top_movewall <= bp->y_pos && bp->y_pos <= movewall_len+top_movewall){
			bp->x_dir = -1;
			return_val = 1;
		}else{
			printf("you lose\n");
			fflush(stdout);
			wrap_up();
			exit(1);
		}
	}

	return return_val;
}

int set_ticker(int secs){
	struct itimerval new_timeset;
	long n_sec,n_usec;

	n_sec = secs / 1000;
	n_usec = (secs % 1000) * 1000;

	new_timeset.it_value.tv_sec = n_sec;
	new_timeset.it_value.tv_usec = n_usec;
	new_timeset.it_interval.tv_sec = n_sec;
	new_timeset.it_interval.tv_usec = n_usec;

	return setitimer(ITIMER_REAL,&new_timeset,NULL);
}
