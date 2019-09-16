#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#define QUESTION "Do you want another transaction"

int get_response(char *);
void tty_mode(int);
void set_crmode();

int main(){
	int response;
	tty_mode(0);
	set_crmode();
	response = get_response(QUESTION);
	tty_mode(1);
	return response;
}

void set_crmode(){
	struct termios mode;
	tcgetattr(0,&mode);
	mode.c_lflag &= ~ICANON;
	mode.c_lflag &= ~ECHO;
	mode.c_cc[VMIN] = 1;
	tcsetattr(0,TCSANOW,&mode);
}

void tty_mode(int flag){
	static struct termios original_mode;

	if(flag == 0){
		tcgetattr(0,&original_mode);
	}else{
		tcsetattr(0,TCSANOW,&original_mode);
	}
}

int get_response(char *word){
	int input;

	printf("%s (y/n)?",word);

	while(1){
		switch((input = getchar())){
			case 'y':
			case 'Y':
				return 0;
			case 'n':
			case 'N':
			case EOF:
				return 1;
		}
	}
}
