//简单的shell可以执行含参数指令,并且实现管道功能，管道符前后要留有空格如 "ls | wc"
//使用了《深入理解计算机系统》中的库<csapp.h>，因此编译时需要加上-lpthread
//如gcc -o DupShell DupShell.c -lpthread
//刘琎
//2019/3/25
#include<pwd.h>
#include <csapp.h>
#define MAXARGS 128
/*函数原型*/
void Parse(char* input, char* output[]);
void type_prompt();
void ParseArg(char* input, char* output[]); 
void Exec(char* argv[]);  //创建子进程执行进程程序替换
int IsPipe(char* argv[]);  //判断指令是否包含管道
void ParsePipe(char* input[], char* output1[],char* output2[]);//用于将input按照|进行切分，最后一个后面为当前路径
void ExecvPipe(char* argv1[],char* argv2[]);

int main()
{
    while(1)
    {
        //首先显示shell提示
	    type_prompt();
        //从标准输入读取字符串
        char buf[1024] = {0};
		Fgets(buf, MAXLINE, stdin);
			if (feof(stdin)) exit(0);
        //解析字符串，解析出指令和参数
        char* argv[100];
        ParseArg(buf,argv);

        //判断解析出来的字符串数组里面是否含有管道                                                                                      
        int ret1 = IsPipe(argv);
        if(ret1 > 0)
        {
            //是管道
            char* argv1[10];
            char* argv2[10];
            ParsePipe(argv,argv1,argv2);
            ExecvPipe(argv1,argv2);
        }
        else
        {
            Exec(argv);
        }
    }
    return 0;
}

void Parse(char* input, char* output[])
{
    char* p = strtok(input,"/");
    int i = 0;
    while(p!= NULL)
    {
        output[i++] = p;
        p=strtok(NULL,"/");
    }
}
void type_prompt()
{
	char path[100] = {0};
	getcwd(path,sizeof(path)); //getcwd获得的是绝对路径(path里面存放的是绝对路径)
	char* path2[100] = {0};                                                                                                             
	Parse(path,path2);

	char* path3 = NULL;  //path里面为绝对路径，将path按照/进行分割，分割的结果保存在path2里面,path3里面保存最后一个字符串，即相对路>径                                                                                                                                      
	int i = 0;
	while(path2[i] !=NULL)
	{
		path3= path2[i];
		i++;
	}      

	struct passwd *pwd;
	pwd = getpwuid(getuid());
	//获得主机名
	char hostname[100] ={0};
	gethostname(hostname,sizeof(hostname));
	printf("[DupShell]%s@%s: %s$  ",pwd->pw_name,hostname,path3);
	fflush(stdout);
}
//用于对输入的命令进行切分
void ParseArg(char* input, char* output[])  
{
	input[strlen(input)-1] = ' ';//把换行符改成空格
    char* p = strtok(input," ");
    int output_size = 0;
    while(p != NULL)
    {
        output[output_size++] = p;
        p = strtok(NULL," ");
    }
    output[output_size] = NULL;
}

void Exec(char* argv[])  //创建子进程执行进程程序替换
{
    pid_t pid = fork();
    if(pid == -1)                                                                                                                       
    {
        perror("fork()");
    }
    else if(pid == 0)
    {
		if(execvp(argv[0], argv) < 0)
		{
			printf("%s: Command not found.\n",argv[0]);
			exit(0);
		}
    }
    else
    {
        //parent
        wait(NULL);
    }
}

//判断指令是否包含管道
int IsPipe(char* argv[])
{
    int i = 0;
    while(argv[i] != NULL)
    {
        if(strcmp(argv[i], "|") == 0)
        {
            return i+1; //i是管道，则i+1就是管道的下一个命令
        }
        ++i;
    }
    return 0;
}

void ParsePipe(char* input[], char* output1[],char* output2[])//用于将input按照|进行切分，最后一个后面为当前路径                    

{
    int i = 0;
    int size1 = 0;
    int size2 = 0;                                                                                                                      
    int ret = IsPipe(input);

    while(strcmp(input[i], "|")!=0)
    {
        output1[size1++] = input[i++];
    }
     output1[size1] = NULL;

     int j = ret;//j指向管道的后面那个字符
     while(input[j] != NULL)
     {
         output2[size2++] = input[j++];
     }

     output2[size2] = NULL;
}


void ExecvPipe(char* argv1[],char* argv2[])
{
    pid_t pid = fork();
    if(pid == -1)
    {                                                                                                                           
        perror("fork()");
    }                                                                                                                                   
    else if(pid == 0)
    {
        //child,创建一个管道
        int fd[2];
        int ret = pipe(fd);
        if(fork() == 0 )
        {
            //子进程创建一个子进程，用于执行管道之前的命令
            close(fd[0]); //孙子进程关闭掉读端
            dup2(fd[1],1);  //将标准输出作为管道的标准输入
            execvp(argv1[0],argv1);
        }
        wait(NULL);

        close(fd[1]); //关闭掉写端
        dup2(fd[0],0);
        execvp(argv2[0],argv2);
        //execvp失败会执行下面的语句
        perror("execvp()");
        exit(0);    
    }
    else                                                                                                                                
    {
        //parent
        wait(NULL);
    }
}


