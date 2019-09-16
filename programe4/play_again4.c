#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

#define ASK "Do you want another transaction"
#define TRIES 3
#define SLEEPTIME 2
#define BEEP putchar('\a')

void tty_mode(int);
void set_cr_noecho_mode();
void set_nodelay_mode();
int get_response(char *,int );
int get_ok_char();
void ctrl_c_handler(int);

int main(){
	int response;
	
	tty_mode(0);
	set_nodelay_mode();
	set_cr_noecho_mode();
	signal(SIGINT,ctrl_c_handler);
	signal(SIGQUIT,SIG_IGN);
	response = get_response(ASK,TRIES);
	tty_mode(1);

	return response;
}

void tty_mode(int flag){
	static struct termios original_mode;
	static int original_flags;
	static int stored = 0;

	if(flag == 0){
		tcgetattr(0,&original_mode);
		original_flags = fcntl(0,F_GETFL);
		stored = 1;
	}

	if(stored == 1){
		tcsetattr(0,TCSANOW,&original_mode);
		fcntl(0,F_SETFL,original_flags);
	}
}

void set_cr_noecho_mode(){
	struct termios ttystate;
	tcgetattr(0,&ttystate);
	ttystate.c_lflag &= ~ICANON;
	ttystate.c_lflag &= ~ECHO;
	ttystate.c_cc[VMIN] = 1;
	tcsetattr(0,TCSANOW,&ttystate);
}

int get_response(char *question,int maxtries){
	int input;
	printf("%s (y/n)?",question);
	fflush(stdout);

	while(1){
		sleep(SLEEPTIME);
		input = tolower(get_ok_char());

		if(input == 'y'){
			return 0;
		}else if(input == 'n'){
			return 1;
		}else if(maxtries-- == 0){
			return 2;
		}

		BEEP;
	}
}

void set_nodelay_mode(){
	int termflags;

	termflags = fcntl(0,F_GETFL);
	termflags |= O_NONBLOCK;
	fcntl(0,F_SETFL,termflags);
}

int get_ok_char(){
	int c;

	while((c = getchar()) != EOF && strchr("yYnN",c) == NULL)
		;

	return c;
}

void ctrl_c_handler(int signum){
	tty_mode(1);
	exit(1);
}
