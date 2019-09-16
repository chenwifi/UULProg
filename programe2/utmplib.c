#include <stdio.h>
#include <fcntl.h>
#include <utmp.h>
#include "utmplib.h"

#define NRECS 16 
#define UTSIZE (sizeof(struct utmp))
#define NULLUT ((struct utmp*) NULL)

static int fd_utmp = -1;
static int nums;
static int cur;
static char utmpbuf[NRECS * UTSIZE];

int utmp_open(char *filename){
	fd_utmp = open(filename,O_RDONLY);
	cur = nums = 0;
	return fd_utmp;
}

struct utmp* utmp_next(){
	struct utmp *tmp;
	if(cur==nums && utmp_reload()==0){
		return NULLUT;
	}

	tmp = (struct utmp *)&utmpbuf[cur++ * UTSIZE];
	return tmp;
}

int utmp_reload(){
	int count;
	count = read(fd_utmp,utmpbuf,NRECS * UTSIZE);
	nums = count/UTSIZE;
	cur = 0;
	return nums;
}

void utmp_close(){
	if(fd_utmp != -1){
		close(fd_utmp);
	}
}
