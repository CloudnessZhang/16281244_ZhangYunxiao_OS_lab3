#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

sem_t *x1_signal=NULL;
sem_t *x2_signal=NULL;
sem_t *x3_signal=NULL;

int main()
{
       	pid_t P2,P3,P4;
        x1_signal=sem_open("x1_signal",O_CREAT,0666,1);//控制P2与P3之间的互斥访问，信号量初始值为0
        x2_signal=sem_open("x2_signal",O_CREAT,0666,0);//控制P2与P4之间的先后访问，信号量初始值为0
        x3_signal=sem_open("x3_signal",O_CREAT,0666,0);//控制P3与P4之间的先后访问，信号量初始值为0

        P2=fork();//创建进程P2
        if(P2<0)
        {
                perror("创建进程P2出错！");
       	}
	if(P2==0)
	{
		//当前进程为P2
		sem_wait(x1_signal);//实现P2、P3互斥访问，锁定信号量x1
		printf("I am the process P2!\n");
		sem_post(x1_signal);//实现P2、P3互斥访问，释放信号量x1
		sem_post(x2_signal);//实现P2、P4先后访问，P2完成后释放x2信号
	}
	if(P2>0)
	{
		//当前进程为P1
		P3=fork();//创建P3进程
		if(P3<0)	
		{
			perror("创建进程P3出错！");
		}
	if(P3==0)
		{
			//当前进程为P3
			sem_wait(x1_signal);//实现P2、P3互斥访问，锁定信号量x1
			printf("I am the process P3!\n");
			sem_post(x1_signal);//实现P2、P3互斥访问，释放信号量x1
			sem_post(x3_signal);//实现P3、P4先后访问，P3完成后释放x3信号
		}
		if(P3>0)
		{
			//当前进程为P1
			printf("I am the process P1!\n");//由于P1是P2、P3、P4的父进程所以P1一定会是第一个执行
			P4=fork();//创建P4进程
			if(P4<0)	
			{
				perror("创建进程P4出错！");
			}
			if(P4==0)
			{
				//当前进程为P4
				sem_wait(x2_signal);//实现P2与P4先后访问，等待x2信号
				sem_wait(x3_signal);//实现P3与P4先后访问，等待x3信号
				printf("I am the process P4!\n");
			}
		}

	}
	sem_close(x1_signal);
	sem_close(x2_signal);
	sem_close(x3_signal);
	sem_unlink("x1_signal");
	sem_unlink("x2_signal");
	sem_unlink("x3_signal");
	return 0;
}
