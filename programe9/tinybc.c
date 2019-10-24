#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void be_dc(int todc[2],int fromdc[2]);
void be_bc(int todc[2],int fromdc[2]);

int main(){
	int pid,todc[2],fromdc[2];

	if(pipe(todc) == -1 || pipe(fromdc) == -1){
		perror("pipe");
		exit(1);
	}

	if((pid = fork()) == -1){
		perror("fork");
		exit(1);
	}

	if(pid == 0){
		be_dc(todc,fromdc);
	}else{
		be_bc(todc,fromdc);
		wait(NULL);
	}

	return 0;
}

void be_dc(int todc[2],int fromdc[2]){
	if(dup2(todc[0],0) == -1){
		perror("dup2");
		exit(1);
	}
	close(todc[0]);
	close(todc[1]);

	if(dup2(fromdc[1],1) == -1){
		perror("dup2");
		exit(1);
	}
	close(fromdc[0]);
	close(fromdc[1]);
	execlp("dc","dc","-",NULL);
	perror("execlp");
	exit(1);
}

void be_bc(int todc[2],int fromdc[2]){
	FILE *fpout,*fpin;
	char message[BUFSIZ],operation[BUFSIZ];
	int num1,num2;

	close(todc[0]);
	close(fromdc[1]);

	fpout = fdopen(todc[1],"w");
	fpin = fdopen(fromdc[0],"r");

	if(fpout == NULL || fpin == NULL){
		perror("fdopen");
		exit(1);
	}

	while(fgets(message,BUFSIZ,stdin) != NULL){
		if(sscanf(message,"%d%[+-*/^]%d",&num1,operation,&num2) != 3){
			printf("syntax error\n");
			continue;
		}

		fprintf(fpout,"%d\n%d\n%c\np\n",num1,num2,*operation);
		fflush(fpout);

		if(fgets(message,BUFSIZ,fpin) == NULL){
			break;
		}
		printf("%d%c%d = %s",num1,*operation,num2,message);
	}

	fclose(fpout);
	fclose(fpin);
}
