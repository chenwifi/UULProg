#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/msg.h>

#define MAX_LEN 100
struct msgbuf{
	long mtype;
	char mtext[MAX_LEN];
};

int main(){
	int running = 1;
	int msgid;
	char buffer[BUFSIZ];
	struct msgbuf data;

	msgid = msgget((key_t)1234,IPC_CREAT | IPC_EXCL | 0666);
	if(msgid == -1){
		fprintf(stderr,"msgget error\n");
		exit(1);
	}

	while(running){
		printf("Enter some text: ");
		fgets(buffer,BUFSIZ,stdin);
		data.mtype = 1;
		strcpy(data.mtext,buffer);
		if(msgsnd(msgid,(void *)&data,MAX_LEN,0) == -1){
			fprintf(stderr,"msgsnd error\n");
			exit(1);
		}

		if(strncmp(data.mtext,"end",3) == 0){
			running = 0;
		}

		sleep(1);
	}

	return 0;
}
