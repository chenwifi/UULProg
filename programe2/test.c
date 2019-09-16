#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int func(int n){
	int sum = 0,i;
	for(i=0; i<=n; i++){
		sum += n;
	}

	return sum;
}

int main(){
	FILE *fp = stdin;

	int i, len = 10;
	long result = 0;

	int *array = (int *)malloc(len * sizeof(int));
	for(i=0;i<len;i++){
		*(array+i) = i;
	}

	for(i=0;i<=100;i++){
		result += i;
	}
	printf("result 1-100: %d\n",result);
	printf("result 1-200: %d\n",func(200));

	return 0;
}
