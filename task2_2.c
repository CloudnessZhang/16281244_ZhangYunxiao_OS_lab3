#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>

volatile int ticketCount=1000;

//sem_t *mutex=NULL;

void *Sale(void *arg){
        int num,temp;
        num=atoi(arg);

        for(int i=0;i<num;i++){
                //sem_wait(mutex);//上锁
                temp=ticketCount;
                pthread_yield();
                temp=temp-1;
                pthread_yield();
                ticketCount=temp;
                //sem_post(mutex);//解锁
        }
}

void *Refund(void *arg){
        int num,temp;
        num=atoi(arg);

        for(int i=0;i<num;i++){
                //sem_wait(mutex);//上锁
                temp=ticketCount;
                pthread_yield();
                temp=temp+1;
                pthread_yield();
                ticketCount=temp;
                //sem_post(mutex);//解锁
 	}

}

int main(int argc,char *argv[]){
        if(argc!=3){
                printf("请输入正确参数！\n");
                exit(0);
        }
        //mutex = sem_open("mutex",O_CREAT,0666,1);//设置互斥访问信号量，其初始值为1
        printf("初始化总票数为：%d\n",ticketCount);
        printf("总售票票数为：%s\n",argv[1]);
        printf("总退票票数为：%s\n",argv[2]);
        pthread_t P1,P2;
        pthread_create(&P1,NULL,Sale,argv[1]);
        pthread_create(&P2,NULL,Refund,argv[2]);
        pthread_join(P1,NULL);
        pthread_join(P2,NULL);
        printf("最终票数为：%d\n",ticketCount);
        //sem_close(mutex);
        //sem_unlink("mutex");
        return 0;
}

