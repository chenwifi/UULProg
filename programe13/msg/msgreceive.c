#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>

struct msgbuf{
	long mtype;
	char mtext[BUFSIZ];
};

int main(){
	int running = 1;
	int msgid;
	struct msgbuf data;

	msgid = msgget((key_t)1234,IPC_CREAT | 0666);
	if(msgid == -1){
		fprintf(stderr,"msgget error\n");
		exit(1);
	}

	while(running){
		if(msgrcv(msgid,(void *)&data,BUFSIZ,2,0) == -1){
			fprintf(stderr,"msgrcv error\n");
			exit(1);
		}

		printf("you wrote %s\n",data.mtext);
		if(strncmp(data.mtext,"end",3) == 0){
			running = 0;
		}
	}

	if(msgctl(msgid,IPC_RMID,NULL) == -1){
		fprintf(stderr,"msgctl error\n");
		exit(1);
	}

	return 0;
}
