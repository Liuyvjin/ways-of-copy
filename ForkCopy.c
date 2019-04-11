/************************************************
*@file  	ForkCopy.c
*@brief     创建两个进程，一个读取文件，另一个写入文件，通过
*           exec交流
*@author   刘琎    516020910128
*@date     2019/3/20
************************************************/
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/wait.h>

#define MAX_SIZE 100
#define DEF_MODE S_IRUSR|S_IWUSR //文件权限模式用户可读写

char buff[MAX_SIZE];
int len = 0;

/**
 * @brief 输出错误信息
 * @param str 输出的信息
 */
void sys_err(const char *str) {
    perror(str);
    exit(-1);
}

//主函数
int main(int argc, char **argv) {
    //检查参数个数
    if (argc < 3)
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
    if (src_fd == -1)
        sys_err("open file fail");

    //读入数据到buff
    while( (len = read(src_fd, buff, MAX_SIZE)) != 0)
    {
        //将len转化成字符串
        char lenstr[10];
        sprintf(lenstr, "%d", len);
        //fork + exec
        pid = fork();
        if (pid < 0)
            sys_err("fork error");
        else if (pid == 0)    //子进程用exec调用程序以追加方式写入文件
        {
            char cmd_name[20] = "./write_to_file"; //写入程序的名称
            char *arg[] = {cmd_name, argv[2], buff, lenstr, NULL};//参数列表
            if (execvp(cmd_name, arg) < 0)//通过execvp调用
            {
                sys_err("execve error");
                exit(-1);
            }
            exit(0);
        }
        waitpid(pid, &status, 0);
    }

    close(src_fd);
    printf("copy success!\n");
    return 0;
}
