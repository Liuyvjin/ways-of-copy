# 操作系统第一次作业
刘琎  
516020910128
## 文件列表
1. sig.c  
信号实验程序，程序间隔1s打印一条sleep信息，当收到SIGINT之后每隔2s打印一条hello
2. MyCopy.c  
使用system calls，实现通过./MyCopy srcFile DesFile 将srcFile复制到 DesFile 中
3. ForkCopy.c  
创建两个进程，一个读取文件，另一个写入文件，通过exec交流实现复制文件
4. PipeCopy.c  
创建两个进程，一个读取文件，另一个写入文件，通过Pipe交流实现复制文件
5. SockCopy.c  
创建两个进程，一个读取文件，另一个写入文件，通过Socket交流实现文件复制  
由于使用了《深入理解计算机系统》提供的库<csapp.h>，因此编译时需要加上-lpthread，例如：  
`gcc -o SockCopy SockCopy.c -lpthread`
6. MyShell.c  
简单的shell可以执行无参数指令，若执行含参数指令会报错。  
编译命令：`gcc -o MyShell MyShell.c include/csapp.c -lpthread`
7. MoreShell.c  
简单的shell可以执行含有参数指令。  
编译命令：`gcc -o MoreShell MoreShell.c include/csapp.c -lpthread`
8. DupShell.c  
简单的shell可以执行含参数指令,并且实现管道功能，管道符前后要留有空格如`ls | wc`
编译命令：`gcc -o DupShell DupShell.c include/csapp.c -lpthread`
9. makefile  
makefile文件。
10. include/csapp.h and include/csapp.c  
《深入理解计算机系统》提供的头文件`csapp.h`以及实现文件`csapp.c`
11. srcFile.txt
用于测试复制功能的输入文件。
