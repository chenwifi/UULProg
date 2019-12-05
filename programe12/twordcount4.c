#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct arg_set{
	char *fname;
	int count;
};

struct arg_set *mailbox;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t flag = PTHREAD_COND_INITIALIZER;

void *count_words(void *arg);

int main(int argc,char *argv[]){
	struct arg_set args1,args2;
	pthread_t t1,t2;
	int reports_in = 0;
	int total_words = 0;

	if(argc != 3){
		printf("usage: %s file1 file2\n",argv[0]);
		exit(1);
	}

	pthread_mutex_lock(&lock);

	args1.fname = argv[1];
	args1.count = 0;
	pthread_create(&t1,NULL,count_words,(void *)&args1);
	args2.fname = argv[2];
	args2.count = 0;
	pthread_create(&t2,NULL,count_words,(void *)&args2);

	while(reports_in < 2){
		pthread_cond_wait(&flag,&lock);
		printf("%s:%d\n",mailbox->fname,mailbox->count);
		total_words += mailbox->count;
		if(mailbox == &args1){
			pthread_join(t1,NULL);
		}
		if(mailbox == &args2){
			pthread_join(t2,NULL);
		}

		reports_in++;
		mailbox = NULL;
		pthread_cond_signal(&flag);
	}
	pthread_mutex_unlock(&lock);

	printf("total words count is: %d\n",total_words);

	return 0;
}

void *count_words(void *arg){
	struct arg_set *tmp = arg;
	FILE *fp;
	int c,prevc = '\0';

	if((fp = fopen(tmp->fname,"r")) != NULL){
		while((c = fgetc(fp)) != EOF){
			if(!isalnum(c) && isalnum(prevc)){
				tmp->count++;
			}
			prevc = c;
		}
		tmp->count++;
		fclose(fp);
	}

	pthread_mutex_lock(&lock);
	if(mailbox != NULL){
		pthread_cond_wait(&flag,&lock);
	}
	mailbox = tmp;
	pthread_cond_signal(&flag);
	pthread_mutex_unlock(&lock);

	return NULL;
}
