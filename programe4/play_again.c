#include <stdio.h>

#define QUESTION "Do you want another transaction"

int get_response(char *);

int main(){
	int response;
	response = get_response(QUESTION);
	return response;
}

int get_response(char *word){
	printf("%s (y/n)?",word);

	while(1){
		switch(getchar()){
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
