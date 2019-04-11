//创建两个进程，一个读取文件，一个写入文件，通过pipe交流
//刘琎
//2019/3/20
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/wait.h>
#include<signal.h>
#define MAX_SIZE 200
#define DEF_MODE S_IRUSR|S_IWUSR

int flag = 1; //结束复制标志

void sys_err(const char *str)
{
    perror(str);
    exit(1);
}

void sigint_handler(int sig)
{
	flag=0;
}

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
	char const *des_path = argv[2];
	pid_t pid;
	int pipe_fd[2];
	char buff[MAX_SIZE];
	int src_fd, des_fd;
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
	else if(pid == 0) //子进程
	{
		close(pipe_fd[1]);
				
		//打开写入文件
		des_fd = open(des_path, O_RDWR|O_CREAT|O_TRUNC, DEF_MODE);
		if(des_fd == -1)
			sys_err("open file fail");
			
		//读入数据到buff
		while(1)
		{
			len=read(pipe_fd[0], buff, MAX_SIZE);
			write(des_fd, buff, len);	//写入数据到目标文件
			if(len == 0 && flag==0)break;
		}
		close(des_fd);
		close(pipe_fd[0]);
		exit(0);
	}
	else  //父进程
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
	printf("Done!\n");
	return 0;
}
