

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<winsock2.h>

const int port = 6012; //����һ��port���������ڴ��ݶ˿ں�
const char* ipserver = "127.0.0.1";

void Error_Handling(const char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hServSock;
	SOCKADDR_IN servAddr;

	int strLen = 0;
	char message[30];
	int idx = 0, readLen = 0;//��֤TCP�������ݲ��������ݱ߽綨���

	/*
	 ��δ������ã�����
	*/
	//if (argc != 3) {
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
	servAddr.sin_addr.s_addr = inet_addr(ipserver);
	servAddr.sin_port = htons(port);

	if (connect(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		Error_Handling("connect() error");


	/*
		int recv(SOCKET s, const char * buf, int len, int flags);
		�������ã���������
		����������s ��ʾ���ݽ��ն������ӵ��׽��־��ֵ
				  buf ������������ݵĻ����ֵַ
				  len Ҫ���յ��ֽ���
				  flags �����������õ��Ķ���ѡ����Ϣ
		�������أ� �ɹ�ʱ���ؽ����ֽ�����ʧ��ʱ����SOCKET_ERROR;
		ע�����recv ��������windows���У�Linux��Ҳ��ͬ���ĺ�������BSD�׽���
				  ��Ҫ����ΪLinux�е�read\write������Ӧ��win��send\recv����
	*/
	//strLen = recv(hServSock,message, sizeof(message) - 1 , 0);
	//if (strLen == -1)
	//	Error_Handling("read() error");
	
	
	//���δ���������֤TCP��������ݲ��������ݱ߽磨write �� read���ô�����ͬ��
	while(readLen = recv(hServSock, &message[idx++], 1, 0))
	{
		if(-1 == readLen)			//��ô�ܱ��� ����readLen = -1 ����д�� Ӧ������д��-1 == readLen��									
			Error_Handling("read() error");
		strLen += readLen;
		//if(message[idx-1] == '\0') //��ʱ�����ݴ������
		//	break;
	}
	

	printf("message from server: %s \n", message);
	printf("Function read call count: %d", strLen);

	
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



