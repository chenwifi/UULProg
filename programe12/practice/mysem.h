typedef struct mysem{
	int value;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
}mysem;
int mysem_destroy(mysem *sem);
int mysem_post(mysem *sem);
int mysem_wait(mysem *sem);
int mysem_init(mysem *sem,unsigned int value);
