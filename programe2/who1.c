#include <stdio.h>
#include <stdlib.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include "utmplib.h"

#define SHOWHOST

void show_info(struct utmp*);
void show_time(long);

int main(){
	struct utmp * current_recode;
	int utmpfd;
	int reclen = sizeof(struct utmp);

	/*
	if((utmpfd = open(UTMP_FILE,O_RDONLY)) == -1 ){
		perror(UTMP_FILE);
		exit(1);
	}
	*/
	if((utmpfd = utmp_open(UTMP_FILE)) == -1 ){
		perror(UTMP_FILE);
		exit(1);
	}

	while((current_recode = utmp_next()) != (struct utmp *)NULL){
		show_info(current_recode);
	}
	//close(utmpfd);
	utmp_close();
	return 0;
}

void show_info(struct utmp *utbufp){
	if(utbufp->ut_type != USER_PROCESS){
		return ;
	}

	printf("%-8.8s",utbufp->ut_name);
	printf(" ");
	printf("%-8.8s",utbufp->ut_line);
	printf(" ");
	//printf("%10ld",utbufp->ut_time);
	show_time(utbufp->ut_time);
	printf(" ");
#ifdef SHOWHOST
	printf("(%s)",utbufp->ut_host);
#endif
	printf("\n");
}

void show_time(long timeval){
	struct tm *cp;
	cp = gmtime(&timeval);
	printf("%d-",cp->tm_year + 1900);
	++cp->tm_mon >= 10 ? printf("%d-",cp->tm_mon) : printf("0%d-",cp->tm_mon);
	printf("%d",cp->tm_mday);
	printf("  ");
	cp->tm_hour >= 10 ? printf("%d:",cp->tm_hour) : printf("0%d:",cp->tm_hour);
	cp->tm_min >= 10 ? printf("%d:",cp->tm_min) : printf("0%d:",cp->tm_min);
}
