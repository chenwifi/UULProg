#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <pthread.h>

int total_words;

void *count_words(void *f);
int main(int argc,char *argv[]){
	pthread_t t1,t2;

	if(argc != 3){
		printf("usage: %s filename1 filename2\n",argv[0]);
		exit(1);
	}

	total_words = 0;

	pthread_create(&t1,NULL,count_words,(void *)argv[1]);
	pthread_create(&t2,NULL,count_words,(void *)argv[2]);
	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
	printf("total_words: %d\n",total_words);

	return 0;
}

void *count_words(void *f){
	char *filename = (char *)f;
	FILE *fp;
	int c,prevc = '\0';

	if((fp = fopen(filename,"r")) != NULL){
		while((c = fgetc(fp)) != EOF){
			if(!isalnum(c) && isalnum(prevc)){
				total_words++;
			}
			prevc = c;
		}
		total_words++;
	}

	return NULL;
}
