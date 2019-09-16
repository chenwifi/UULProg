#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <termios.h>

int done = 0;

void on_input(int);
void enable_kbd_signals();
void set_crmode();
void tty_mode(int flag);

int main(){
	//tty_mode(0);
	signal(SIGIO,on_input);
	enable_kbd_signals();
	//set_crmode();
	int i = 0;

	while(! done){
		i++;
		printf("the num is %d\n",i);
		fflush(stdout);
	}
	//tty_mode(1);

	return 0;
}

void enable_kbd_signals(){
	int fd_flags;
	fcntl(0,F_SETOWN,getpid());
	fd_flags = fcntl(0,F_GETFL);
	fcntl(0,F_SETFL,(fd_flags | O_ASYNC | O_NONBLOCK));
}

void tty_mode(int flag){
	static struct termios original_mode;

	if(flag == 0){
		tcgetattr(0,&original_mode);
	}else{
		tcsetattr(0,TCSANOW,&original_mode);
	}
}

void set_crmode(){
	struct termios mode;
	tcgetattr(0,&mode);
	mode.c_lflag &= ~ICANON;
	mode.c_lflag &= ~ECHO;
	mode.c_cc[VMIN] = 1;
	tcsetattr(0,TCSANOW,&mode);
}

void on_input(int signum){
	int c;
	c = getchar();
	//printf("%c",c);
	if(c == 'q'){
		done = 1;
	}
}
