#define LEN 50

typedef struct data{
	char time[LEN];
	int from;
}timedata;

int sharememory(int size,int flag);
int creat_ipc(int size);
int get_ipc(int size);
int destroy_sharememory(int id);
