#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#define MAX_SIZE 256
#define I_NUMBER 5


FILE *fp;
char str[MAX_SIZE] = "Operaing system";

void *writer(void *arg){
	int i;
	int cnt = 0;
	char org_str[512];
	char temp_str[512];
	//strcpy(org_str,str);
	//char temp_cnt[10];
	//sprintf(temp_cnt,"%d",cnt);
	//for(i=0;i<I_NUMBER;i++){
	usleep(1);
	//sprintf(temp_cnt,"%d",cnt);
	strcpy(temp_str, strcat(str," good"));
	printf("Writer %d writes %s behind string\n",(*((int *)arg)),"good");
	strcpy(str,temp_str);
	
	//}
}

void *reader(void *arg){
	int i;
	//for(i=0;i<I_NUMBER;i++){
	usleep(1);
	printf("Reader %d reads string %s\n",(*((int *)arg)),str);
	//}
}

int main(){
	int i = 0;
	int a[10] = {1,2,3,4,5,6,7,8,9,10};
	printf("Original String : Operaing sytem, write String : good\n");
	pthread_t write_threads[10];
	pthread_t read_threads[10];

	for(i=0;i<10;i++){
		pthread_create(&write_threads[i],NULL,writer,(void *)&a[i]);
		pthread_create(&read_threads[i],NULL,reader,(void *)&a[i]);
	}

	for(i=0;i<10;i++){
		pthread_join(write_threads[i],NULL);
		pthread_join(read_threads[i],NULL);
	}


	
	return 0;
}

