#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<winsock2.h>

const int port = 6012; //定义一个port常量，用于传递端口号

void Error_Handling(const char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAddr, clntAddr;
	int szClntAddr;
	char message[] = "Hello world!!!";

	/*
	 这段代码作用？？？
	*/
	//if (argc != 2) {
	//	printf("Usage: %s <port>\n", argv[0]);
	//	exit(1);
	//}

	/*
	    WSAStartup(WORD wVersionRequested, LPWSADATA lpWSAData)
		函数作用：设置程序中用到的Winsock版本，并初始化相应版本的库
		函数参数：第一个参数为套接字版本信息（WORD类型） 
		          例：MAKEWORD（1，2）主版本为1，副版本为2，返回0X0201
				  第二个参数传入WSADATA型结构体变量地址
				  无特殊含义，但是为了调用函数，必须传递
		函数返回值：成功时返回 0 失败时返回非零的错误代码值
	*/
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		Error_Handling("WSAStartup() error!");


	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	if (hServSock == INVALID_SOCKET) {
		Error_Handling("socket() error");
	}

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(port);

	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		Error_Handling("bind() error");
	if (listen(hServSock, 5) == SOCKET_ERROR)
		Error_Handling("listen() error");

	szClntAddr = sizeof(clntAddr);

	hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &szClntAddr);
	if (hClntSock == INVALID_SOCKET)
		Error_Handling("accept() error");

	/*
	    int send(SOCKET s, const char * buf, int len, int flags);
		函数作用：发送数据
		函数参数：s 表示数据传输对象连接的套接字句柄值
		          buf 保存待传输数据的缓冲地址值
				  len 要传输的字节数
				  flags 传输数据是用到的多种选项信息
		函数返回： 成功时返回输出字节数，失败时返回SOCKET_ERROR;
		注意事项：send 函数并非windows独有，Linux中也有同样的函数来自BSD套接字
		          不要误认为Linux中的read\write函数对应于win中send\recv函数
	*/
	send(hClntSock, message, sizeof(message), 0);

	closesocket(hClntSock);
	closesocket(hServSock);
	WSACleanup();
	return 0;

}


void Error_Handling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}



