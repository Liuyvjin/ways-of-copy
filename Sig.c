/**********************************************
*@file	    Sig.c
*@brief     程序间隔1s打印一条sleep信息，当收到SIGINT之后
*                每隔2s打印一条hello
*@author   刘琎 516020910128
*@date       2019/3/20
***********************************************/
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

volatile sig_atomic_t hello_flag = 0;//是否收到信号标志

/**
*@brief    信号处理函数
*/
void sigint_handler(int sig)
{
	hello_flag = 1;
}

void main(int argc, char **argv)
{
	int count = 0; //计时器,程序运行15s后自动停止
	if(signal(SIGINT, sigint_handler) == SIG_ERR)//注册信号处理函数
		printf("signal error"); 
	while(count++<15)
	{
		sleep(1);
		printf("%ds:\tsleep\n",count);   
		if(count%2==0 && hello_flag)
		{
			printf("%ds:\thello\n",count);
		}	
	}
	exit(0);
}


