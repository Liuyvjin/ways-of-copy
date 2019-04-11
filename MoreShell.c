/************************************************
*@file  	MoreShell.c
*@brief     简单的shell可以执行含有参数指令
*@note     由于使用了《深入理解计算机系统》提供的库<csapp.h>，
*          因此编译时需要加上-lpthread和csapp.c例如：
*          gcc -o MoreShell MoreShell.c include/csapp.c -lpthread
*@author   刘琎    516020910128
*@date     2019/3/25
************************************************/
#include "include/csapp.h"

#define MAXARGS 128

/**
 * 函数原型
 */
void type_prompt();//输出提示符

void eval(char *cmdline);//执行命令

int parseline(char *buf, char **argv);//解析命令

int builtin_command(char **argv);//是否是内置命令


/**
 * 主程序
 */
int main() {
    char cmdline[MAXLINE];

    while (1) {
        type_prompt();
        Fgets(cmdline, MAXLINE, stdin);
        if (feof(stdin)) exit(0);

        eval(cmdline);
    }
}

void type_prompt() {
    printf(">>> ");
}

void eval(char *cmdline) {
    char *argv[MAXARGS];
    char buf[MAXLINE];
    int bg;
    pid_t pid;

    strcpy(buf, cmdline);//把命令复制到buf中

    bg = parseline(buf, argv);//解析命令，把命令中的单词分开存入argv
    if (argv[0] == NULL)return;

    if (!builtin_command(argv))//若不是内置命令
    {
        if ((pid = fork()) == 0)//子进程
        {
            if (execvp(argv[0], argv) < 0) {
                printf("%s: Command not found.\n", argv[0]);
                exit(0);
            }
        }

        if (!bg)//不在后台运行
        {
            int status;
            if (waitpid(pid, &status, 0) < 0)
                unix_error("waitfg: waitpid error");
        } else printf("%d %s", pid, cmdline);
    }
    return;
}

int builtin_command(char **argv) {
    if (!strcmp(argv[0], "quit"))//中止程序
    {
        kill(-1, SIGINT);
        exit(0);
    }
    if (!strcmp(argv[0], "&"))//忽略&
        return 1;
    return 0;
}

int parseline(char *buf, char **argv) {
    char *delim;//
    int argc;
    int bg;

    buf[strlen(buf) - 1] = ' ';//把换行符改成空格
    while (*buf && (*buf == ' '))buf++;//忽略空格

    argc = 0;
    while ((delim = strchr(buf, ' ')))//
    {
        argv[argc++] = buf;
        *delim = '\0';
        buf = delim + 1;
        while (*buf && (*buf == ' '))buf++;
    }
    argv[argc] = NULL;

    if (argc == 0) return 1;

    if (bg = (*argv[argc - 1] == '&')) argv[--argc] = NULL;

    return bg;
}





