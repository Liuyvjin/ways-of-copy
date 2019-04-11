//编写函数mycopy实现复制一个文件功能，并在主程序调用
//调用参数：源文件名，目标文件名
//刘琎
//2019/3/20

#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<unistd.h>

#define MAX_SIZE 10
#define DEF_MODE S_IRUSR|S_IWUSR

//复制函数
//param src_fd: 源文件标识符
//param des_fd: 目标文件标识符
int copy(int src_fd, int des_fd)
{
	char buff[MAX_SIZE];
	int len = 0;
	//复制文件
	while((len = read(src_fd, buff, MAX_SIZE)) != 0)
	{
		write(des_fd, buff, len);
	}
	return 0;
}

//主程序
int main(int argc, char **argv)
{
	if(argc < 3) //检查参数个数
	{
		printf("input fail\n");
		exit(-1);
	}
	
	char const *src_path = argv[1];
	char const *des_path = argv[2];
	int src_fd, des_fd;

	//打开文件
	src_fd = open(src_path, O_RDONLY);
	if(src_fd == -1)//检查是否出错
	{
		perror("open file");
		exit(-1);
	}

	des_fd = open(des_path, O_RDWR|O_CREAT|O_TRUNC, DEF_MODE);
	if(des_fd == -1)
	{
		perror("open fail");
		exit(-1);
	}

	//复制文件
	copy(src_fd, des_fd);	
	printf("Done!\n");

	//关闭文件
	close(src_fd);
	close(des_fd);
	exit(0);
}



