/************************************************
*@file  	PipeCopy.c
*@brief     创建两个进程，一个读取文件，另一个写入文件，通过
*           Pipe交流
*@author   刘琎    516020910128
*@date     2019/3/20
************************************************/
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/wait.h>
#include<signal.h>
#define MAX_SIZE 100
#define DEF_MODE S_IRUSR|S_IWUSR //文件权限模式

int end_flag = 0; //结束复制标志

/**
 * @brief 输出错误信息
 * @param str 输出的信息
 */
void sys_err(const char *str)
{
    perror(str);
    exit(1);
}

/**
 * @brief 父进程告知子进程文件写入管道结束
 */
void sigint_handler(int sig)
{
	end_flag=1;
}

/**
 * 主程序
 */
int main(int argc, char** argv)
{
	if(argc < 3) //检查参数个数
	{
		printf("input fail\n");
		exit(-1);
	}
	
	//变量设置
	int status;
	char const *src_path = argv[1];
	char const *des_path = argv[2];//文件路径
	pid_t pid;  //进程号
	int pipe_fd[2]; //管道标识符
    int src_fd, des_fd; //文件标识符
	char buff[MAX_SIZE];  //缓存区
	int len = 0;
	
	//创建管道
	if(pipe(pipe_fd) == -1)
		sys_err("pipe error");
	
	if(signal(SIGINT, sigint_handler) == SIG_ERR)
			sys_err("signal error");

	//创建子进程	
	pid = fork();
	if(pid<0)
		sys_err("fork error");	
	else if(pid == 0) //子进程读取管道写入文件
	{
		close(pipe_fd[1]);  //0是从管道读取
				
		//打开写入文件
		des_fd = open(des_path, O_RDWR|O_CREAT|O_TRUNC, DEF_MODE);
		if(des_fd == -1)
			sys_err("open file fail");
			
		//读入数据到buff
		while(1)
		{
			len=read(pipe_fd[0], buff, MAX_SIZE);
			write(des_fd, buff, len);	//写入数据到目标文件
			if(len == 0 && end_flag)break;
		}
		close(des_fd);
		close(pipe_fd[0]);
		exit(0);
	}
	else  //父进程读取文件写入管道
	{
		close(pipe_fd[0]);
		
		//打开读取文件
		src_fd = open(src_path, O_RDONLY);
		if(src_fd == -1)
			sys_err("open file fail");
			
		//读入数据到buff
		while((len = read(src_fd, buff, MAX_SIZE)) != 0)
			write(pipe_fd[1], buff, len);	//写入数据到目标文件
		close(src_fd);
		close(pipe_fd[1]);
		
		kill(pid, SIGINT);
		waitpid(pid, &status, 0);
	}
	printf("copy success!\n");
	return 0;
}
