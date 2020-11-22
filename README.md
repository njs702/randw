# randw

#### 1.Reader/Writer with no_semaphore
```cpp
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
```
Reader/Writer with no_semaphore 프로그램 구현을 위한 헤더파일, 매크로, 전역변수 선언
```cpp
void *reader(void *arg){
	int i;
	//for(i=0;i<I_NUMBER;i++){
	usleep(1);
	printf("Reader %d reads string %s\n",(*((int *)arg)),str);
	//}
}
```
세마포 기능이 없는 reader function이다. 전역 변수로 선언되어 있는 str을 읽어서 출력해준다.
```cpp
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
```
세마포 기능이 없는 consumer function이다. "good" 문자열을 write 하는 기능을 구현했다. strcat 함수를 통해 good 문자열을 기존의 str에 추가하여 덮어씌운다.
```cpp
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

```
main function이다. Producer thread 2개와 Consumer thread 10개를 생성해서 join해 race condition이 발생하는지 확인하도록 했다.
#### 2.Reader/Writer with semaphore
```cpp
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
```
Reader/Writer with no_semaphore 프로그램 구현을 위한 헤더파일, 매크로, 전역변수 선언, 세마포 기능을 활용하기 위한 변수와 mutex를 선언했다.
```cpp
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
```
세마포 기능이 있는 reader function이다. sme_wait을 통해 writer가 critical section에 들어오는 것을 막아 준다. read_count == 0 인 경우(reader가 없는 경우)에만 post와 unlock을 통해 writer가 접근할 수 있게 된다. 
```cpp
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
```
reader와 달리 writer는 단순한 구조를 갖고 있다. sem_wait(&rw_mutex)을 통해 critical section 진입을 막고, sem_post(&rw_mutex)을 통해 진입을 허용하는 구조를 갖고 있다.
