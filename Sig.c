//任务1：程序间隔1s打印一条信息
//当收到SIGINT后间隔两秒打印一条 hello
//刘琎
//2019/3/20

#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

volatile sig_atomic_t hello_flag = 0;//

void sigint_handler(int sig)
{
	hello_flag = 1;
}

void main(int argc, char **argv)
{
	int count = 0; //计时器
	if(signal(SIGINT, sigint_handler) == SIG_ERR)
		printf("signal error");
	while(count++<15)
	{
		sleep(1);
		printf("%ds:sleep\n",count);
		if(count%2==0 && hello_flag)
		{
			printf("hello\n");
		}	
	}
	exit(0);
}


