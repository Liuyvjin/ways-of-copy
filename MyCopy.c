/**********************************************
*@file	   MyCopy.c
*@brief     使用system calls，实现通过./MyCopy srcFile DesFile 将
*                srcCopy 复制到 DesFile 中
*@author   刘琎 516020910128
*@date       2019/3/20
***********************************************/
#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<unistd.h>

#define MAX_SIZE 100
#define DEF_MODE S_IRUSR|S_IWUSR

/**
*@brief    复制函数
*@param src_fd: 源文件标识符
*@param des_fd: 目标文件标识符
*/
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

	//打开源文件
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
	if( !copy(src_fd, des_fd) )	
		printf("copy success!\n");

	//关闭文件
	close(src_fd);
	close(des_fd);
	exit(0);
}



