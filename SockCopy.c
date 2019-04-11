/************************************************
*@file  	SockCopy.c
*@brief     创建两个进程，一个读取文件，另一个写入文件，通过
*           Socket交流
*@note     由于使用了《深入理解计算机系统》提供的库<csapp.h>，
*          因此编译时需要加上-lpthread，例如：
*          gcc -o SockCopy SockCopy.c -lpthread
*@author   刘琎    516020910128
*@date     2019/3/20
************************************************/

#include "include/csapp.h"
#include <sys/un.h>
#define MAX_SIZE 50
#define UNIX_DOMAIN "/tmp/UNIX.domain"


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
 * @brief 服务器准备完毕告知客户端开始工作
 */
void sigusr1_handler(int sig){printf("Client: get to start\n");}//服务器准备完毕，通知客户端进程

/**
 * @brief 主函数
 */
int main(int argc, char **argv)
{
	char const *src_path = argv[1];
	char const *des_path = argv[2];
	int src_fd;
	int des_fd;

	if(signal(SIGUSR1, sigusr1_handler) == SIG_ERR)
			sys_err("signal error");
			
	pid_t pid;
	pid = fork();
	if(pid<0)
		sys_err("pipe error");
	else if(pid == 0)//客户端,写入数据
	{
        printf("Client: I am waiting...\n");
		pause();//等待服务器准备
		int connect_fd;
		struct sockaddr_un srv_addr;
		char snd_buf[MAX_SIZE];
		char rcv_buf[MAX_SIZE];
		int ret;
		int i;
		//打开文件
		des_fd = open(des_path, O_RDWR|O_CREAT|O_TRUNC, DEF_MODE);
		if(des_fd == -1) sys_err("open file fail");
		//创建客户套接字
		connect_fd = socket(PF_UNIX, SOCK_STREAM, 0);
		if(connect_fd < 0)
		{
			perror("client create socket failed");
			return 1;
		}

		//设置服务器 sockaddr_un
		srv_addr.sun_family = AF_UNIX;
		strcpy(srv_addr.sun_path, UNIX_DOMAIN);
		printf("Client: start success\n");
		
		//连接服务器
		printf("Client: connect to Server\n");
		ret = connect(connect_fd, (struct sockaddr*)&srv_addr, sizeof(srv_addr));
		if(ret == -1)
		{
			perror("Client: connect to server failed!");
			close(connect_fd);
			unlink(UNIX_DOMAIN);
			return 1;
		}
		
		//从服务器接受信息并写入文件
		memset(rcv_buf, 0, MAX_SIZE);
		while(1)
		{
			int rcv_num = read(connect_fd, rcv_buf, sizeof(rcv_buf));
			printf("Client: receive message from server (%d bytes) \n", rcv_num);
			write(des_fd,rcv_buf,rcv_num);
			sleep(0.5);
		}
	}
	
	else//服务端，读入数据
	{
		int lsn_fd, apt_fd;
		struct sockaddr_un srv_addr;
		struct sockaddr_un clt_addr;
		socklen_t clt_len;
		int ret;
		int i,snd_num;
		char recv_buf[MAX_SIZE];
		char send_buf[MAX_SIZE];
		//打开读取文件
		src_fd = open(src_path, O_RDONLY);
		if(src_fd == -1)	sys_err("open file fail");
		
		//创建套接字绑定 local IP 和 PORT
		lsn_fd = socket(PF_UNIX, SOCK_STREAM, 0);
		if(lsn_fd < 0)
		{
			perror("can't create communication socket!");
			return 1;
		}

		//创建 local IP 和 PORT
		srv_addr.sun_family = AF_UNIX;
		strncpy(srv_addr.sun_path, UNIX_DOMAIN, sizeof(srv_addr.sun_path) - 1);
		unlink(UNIX_DOMAIN);

		//绑定 sockfd and sockaddr
		ret = bind(lsn_fd, (struct sockaddr*)&srv_addr, sizeof(srv_addr));
		if(ret == -1)
		{
			perror("can't bind local sockaddr!");
			close(lsn_fd);
			unlink(UNIX_DOMAIN);
			return 1;
		}

		//listen lsn_fd, try listen 1
		ret = listen(lsn_fd, 1);
		if(ret == -1)
		{
			perror("can't listen client connect request");
			close(lsn_fd);
			unlink(UNIX_DOMAIN);
			return 1;
		}

		clt_len = sizeof(clt_addr);
		//服务器启动成功
		printf("Server: start success\n");
		sleep(1);
		kill(pid, SIGUSR1);
		//读取文件并传给客户端
		while(1)
		{
			apt_fd = accept(lsn_fd, (struct sockaddr*)&clt_addr, &clt_len);
			if(apt_fd < 0)
			{
				perror("can't listen client connect request");
				close(lsn_fd);
				unlink(UNIX_DOMAIN);
				return 1;
			}

			printf("Server: received a connection\n");//接收连接
			printf("Server: send message to client ...\n");
			memset(send_buf, 0, MAX_SIZE);
						
			while( (snd_num=read(src_fd, send_buf,MAX_SIZE))!=0 )
				snd_num = write(apt_fd, send_buf, snd_num);
			break;
		}
		sleep(1);
		printf("Server: Done!\n");
		kill(pid,SIGINT);
		close(apt_fd);
		close(lsn_fd);
		unlink(UNIX_DOMAIN);
		return 0;

	}
}





