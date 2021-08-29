#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<winsock2.h>

const int port = 6012; //����һ��port���������ڴ��ݶ˿ں�

void Error_Handling(const char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAddr, clntAddr;
	int szClntAddr;
	char message[] = "Hello world!!!";

	/*
	 ��δ������ã�����
	*/
	//if (argc != 2) {
	//	printf("Usage: %s <port>\n", argv[0]);
	//	exit(1);
	//}

	/*
	    WSAStartup(WORD wVersionRequested, LPWSADATA lpWSAData)
		�������ã����ó������õ���Winsock�汾������ʼ����Ӧ�汾�Ŀ�
		������������һ������Ϊ�׽��ְ汾��Ϣ��WORD���ͣ� 
		          ����MAKEWORD��1��2�����汾Ϊ1�����汾Ϊ2������0X0201
				  �ڶ�����������WSADATA�ͽṹ�������ַ
				  �����⺬�壬����Ϊ�˵��ú��������봫��
		��������ֵ���ɹ�ʱ���� 0 ʧ��ʱ���ط���Ĵ������ֵ
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
		�������ã���������
		����������s ��ʾ���ݴ���������ӵ��׽��־��ֵ
		          buf ������������ݵĻ����ֵַ
				  len Ҫ������ֽ���
				  flags �����������õ��Ķ���ѡ����Ϣ
		�������أ� �ɹ�ʱ��������ֽ�����ʧ��ʱ����SOCKET_ERROR;
		ע�����send ��������windows���У�Linux��Ҳ��ͬ���ĺ�������BSD�׽���
		          ��Ҫ����ΪLinux�е�read\write������Ӧ��win��send\recv����
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



