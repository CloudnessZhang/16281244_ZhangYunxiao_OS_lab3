#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdio.h>
#include<unistd.h>

int buf[10];
sem_t *empty=NULL;
sem_t *full=NULL;

void *Producer(void *arg)
{
	
	for(int i=0;i<10;i++)
	{
		sem_wait(empty);
		scanf("%d",&buf[i]);
		sem_post(full);
		if(i==9) i=-1;
	}	
	return NULL;
}

void *Consumer(void *arg)
{
	for(int i=0;i<10;i++)
	{
		sem_wait(full);
		printf("输出：%d\n",buf[i]);
		sem_post(empty);
		sleep(1);
		if(i==9) i=-1;
	}	
	return NULL;
}

int main(int argc,char *argv[])
{
	empty=sem_open("empty",O_CREAT,0666,10);//empty表示空缓冲区数量，其初始值为10
	full=sem_open("full",O_CREAT,0666,0);//full表示满缓冲区数量，其初始值为0
	pthread_t p1,p2;
	pthread_create(&p1,NULL,Producer,NULL);
	pthread_create(&p2,NULL,Consumer,NULL);
	pthread_join(p1,NULL);
	pthread_join(p2,NULL);
	sem_close(empty);
	sem_close(full);
	sem_unlink("empty");
	sem_unlink("full");
	return 0;

}
