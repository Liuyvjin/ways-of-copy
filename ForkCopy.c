//创建两个进程，一个读取文件，一个写入文件，通过exec交流
//刘琎
//2019/3/20
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/wait.h>

#define MAX_SIZE 1024
#define DEF_MODE S_IRUSR|S_IWUSR

extern char ** environ;
char buff[MAX_SIZE];
int len = 0;

void sys_err(const char *str)
{
    perror(str);
    exit(1);
}

int main(int argc, char** argv)
{
	if(argc < 3) //检查参数个数
	{
		printf("input fail\n");
		exit(-1);
	}

	//变量设置
	char const *src_path = argv[1];
	pid_t pid;
	int src_fd;
	int status;
	
	//打开读取文件
	src_fd = open(src_path, O_RDONLY);
	if(src_fd == -1)
	sys_err("open file fail");

	//读入数据到buff
	len = read(src_fd, buff, MAX_SIZE);
	close(src_fd);	
	char lenstr[20];
	sprintf(lenstr, "%d", len);
	//创建子进程	
	pid = fork();
	if(pid<0)
		sys_err("fork error");	
	else if(pid == 0)	//子进程用exec调用程序写入文件
	{
		char filename[20] = "ForkCopy2";
		char *arg[] = {filename, argv[2], buff, lenstr, NULL};
		if(execve(filename, arg, environ)<0)//通过execve调用
		{
			sys_err("execve error");
			exit(-1);
		}
		exit(0);
	}
	waitpid(pid, &status, 0);
	printf("Done!\n");
	return 0;
}
