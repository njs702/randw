#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#define MAX_SIZE 256
#define I_NUMBER 5

char str[MAX_SIZE] = "Operaing system";
sem_t rw_mutex;
pthread_mutex_t mutex;
int read_count = 0;

void *writer(void *arg){
	int i;
	int cnt = 0;
	char temp_str[512];
	char temp_cnt[10];
	sem_wait(&rw_mutex);
	usleep(0.1);
	strcpy(temp_str,strcat(str," good"));
	printf("Writer %d writes %s behind string\n",(*((int *)arg)),"good");
	strcpy(str,temp_str);
	sem_post(&rw_mutex);
}

void *reader(void *arg){
	int i;
	pthread_mutex_lock(&mutex);
	read_count++;
	if(read_count == 1){
		sem_wait(&rw_mutex);
	}
	pthread_mutex_unlock(&mutex);
	usleep(0.1);
	printf("Reader %d reads string %s\n",(*((int *)arg)),str);
	pthread_mutex_lock(&mutex);
	read_count--;
	if(read_count == 0){
		sem_post(&rw_mutex);
	}
	pthread_mutex_unlock(&mutex);
}

int main(){
	int i=0;
	int a[10] = {1,2,3,4,5,6,7,8,9,10};

	printf("Original String : Operaing system, write String : good\n");

	pthread_t write_threads[10];
	pthread_t read_threads[10];
	pthread_mutex_init(&mutex,NULL);
	sem_init(&rw_mutex,0,1);

	for(i=0;i<10;i++){
		pthread_create(&write_threads[i],NULL,writer,(void *)&a[i]);
		pthread_create(&read_threads[i],NULL,reader,(void *)&a[i]);
	}
	for(i=0;i<10;i++){
		pthread_join(write_threads[i],NULL);
		pthread_join(read_threads[i],NULL);
	}
	pthread_mutex_destroy(&mutex);
	sem_destroy(&rw_mutex);
	return 0;
}
