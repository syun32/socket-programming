#include <WinSock2.h>		//socket programming�� �ϱ� ���� library include
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#define BUFSIZE 1024

void ErrorHandling(const char *message);	//Error�� �߻����� �� ������ �Լ�

int main(int argc, char **argv) {

	SOCKET clntSock;
	WSADATA wsaData;
	SOCKADDR_IN servAddr;

	char sAddr[15];
	int sPort;
	int nRcv;
	unsigned int Addr;
	char message[BUFSIZE];

	struct hostent *host;

	printf("Server Address : ");
	gets_s(sAddr);
	printf("Port Number : ");
	gets_s(message);
	sPort = atoi(message);		//server address, port number�� �Է¹���

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)		//���μ������� Winsock DLL�� ����� �ʱ�ȭ
		ErrorHandling("Load WinSock 2.2 DLL Error");	//������ ��� ErrorHandling

	clntSock = socket(AF_INET, SOCK_STREAM, 0);		//client socket ����
	if (clntSock == INVALID_SOCKET)
		ErrorHandling("Socket Error");			//������ ��� ErrorHandling

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(sAddr);
	servAddr.sin_port = htons(sPort);		//TCP/IP ����� �ϱ� ���� �ʱ� ����

	if (connect(clntSock, (struct sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) {		//connect
		ErrorHandling("Connection Error");
	}
	else {
		printf("Connect OK!\nStart...\n");
	}

	while (1) {		//��ȣ �˻� while��
		printf("\nPassword : ");
		gets_s(message);		//��ȣ�� �Է��Ѵ�


		send(clntSock, message, (int)strlen(message), 0);		//server�� �Է��� ��ȣ ����
		nRcv = recv(clntSock, message, sizeof(message) - 1, 0);		//server�κ��� ��ȣ �˻� ��� ����
		if (nRcv == SOCKET_ERROR) {
			printf("Receive Error..\n");
			break;		//���� ������ �ִٸ� ����
		}
		message[nRcv] = '\0';		//message ������ ���ڿ� NULL

		if (strcmp(message, "OK") == 0) {		//��ȣ�� �Ǵٴ� �ǹ��� OK���� ��
			printf("Login Success\n");
			break;			//�α��ο� �����Ͽ� ��ȣ�˻� while�� Ż��
		}
		else
			printf("Login Fail\n");		//��ȣ ����ġ�� ��ȣ�˻� while�� �ݺ�
	}

	while (1) {
		printf("\nSend Command : ");
		gets_s(message);		//������ command �Է�

		if (strcmp(message, "exit") == 0) {		//exit �Է� ��, ������ ���� �� ����
			send(clntSock, message, (int)strlen(message), 0);
			break;
		}

		send(clntSock, message, (int)strlen(message), 0);	//�Է��� command server�� ����

	}

		closesocket(clntSock);		//client socket delete
		WSACleanup();		//winsock ����� ����
		printf("Close Connection..\n");
		_getch();

		return 0;
	}


void ErrorHandling(const char *message) {	//Error�߻� �� winsock ����� �����ϰ� �޽����� ����� �Լ�
	WSACleanup();		
	fputs(message, stderr);
	fputc('\n', stderr);
	_getch();
	exit(1);
}