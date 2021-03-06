/************************************************
*@file  	write_to_file.c
*@brief    将指定字符串写入指定文件中
 *         write_to_file fileName str
*@author   刘琎    516020910128
*@date     2019/3/20
************************************************/
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/wait.h>

#define MAX_SIZE 100
#define DEF_MODE S_IRUSR|S_IWUSR


void sys_err(const char *str)
{
    perror(str);
    exit(1);
}

int main(int argc, char** argv)
{
	if(argc < 4) //检查参数个数
	{
		printf("input fail\n");
		exit(-1);
	}
	
	int len = atoi(argv[3]);
	//变量设置
	char const *des_path = argv[1];
	int  des_fd;

	//打开写入文件
	des_fd = open(des_path, O_RDWR|O_CREAT|O_APPEND, DEF_MODE);
	if(des_fd == -1)
		sys_err("open file fail");
		
	//写入数据
	write(des_fd, argv[2], len);
	
	//关闭文件
	close(des_fd);

	return 0;
}
