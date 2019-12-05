#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curses.h>
#include <pthread.h>

#define MAXMSG 10
#define TUNIT 20000

struct propset{
	char *str;
	int row;
	int delay;
	int dir;
};

pthread_mutex_t mx = PTHREAD_MUTEX_INITIALIZER; 
int setup(int n,char *s[],struct propset *propsp);

void *animate(void *arg);

int main(int argc,char *argv[]){
	int num_msg,i,c;
	pthread_t thrds[MAXMSG];
	struct propset props[MAXMSG];

	if(argc == 1){
		printf("usage:tanimate string...\n");
		exit(1);
	}

	num_msg = setup(argc-1,argv+1,props);

	for(i = 0;i < num_msg;i++){
		pthread_mutex_lock(&mx);
		if(pthread_create(&thrds[i],NULL,animate,&props[i])){
			fprintf(stderr,"error creating thread");
			endwin();
			exit(1);
		}
		pthread_mutex_unlock(&mx);
	}

	while(1){
		c = getch();
		if(c == 'Q'){
			break;
		}
		if(c == ' '){
			for(i = 0;i < num_msg;i++){
				props[i].dir = -props[i].dir;
			}
		}

		if(c >= '0' && c <= '9'){
			i = c - '0';
			if(i < num_msg){
				props[i].dir = -props[i].dir;
			}
		}
	}

	pthread_mutex_lock(&mx);
	for(i = 0;i < num_msg;i++){
		pthread_cancel(thrds[i]);
	}
	endwin();
	pthread_mutex_unlock(&mx);

	return 0;
}

int setup(int n,char *s[],struct propset *propsp){
	int num_msg = n > MAXMSG ? MAXMSG : n;
	int i;

	srand(getpid());
	for(i = 0;i < n;i++){
		propsp[i].str = s[i];
		propsp[i].row = i;
		propsp[i].delay = 1 + (rand() % 15);
		propsp[i].dir = (rand()%2 ? 1 : -1);
	}
	initscr();
	crmode();
	noecho();
	clear();
	mvprintw(LINES-1,0,"'Q' to quit '0'...'%d' to bounce",num_msg - 1);

	return num_msg;
}

void *animate(void *arg){
	struct propset *info = (struct propset *)arg;
	int len = strlen(info->str);
	int col = rand() % (COLS - len - 5);
	int i;

	while(1){
		usleep(info->delay * TUNIT);

		if(col <= 0 && info->dir == -1){
			info->dir = 1;
		}else if(col + len >= COLS && info->dir == 1){
			info->dir = -1;
		}

		pthread_mutex_lock(&mx);
		move(info->row,col <= 0 ? 0 : col-1);
		for(i = 0;i < len + 2;i++){
			addch(' ');
		}
		move(info->row,col);
		addstr(info->str);
		move(LINES-1,COLS-1);
		refresh();
		pthread_mutex_unlock(&mx);

		col += info->dir;

	}
}
