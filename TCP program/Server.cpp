#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>		//socket programming�� �ϱ� ���� library include
#include <conio.h>


#define BUFSIZE 1024
#define PASSWORD "�´� ��й�ȣ"		//��й�ȣ ����

void ErrorHandling(const char *message);	//Error�� �߻����� �� ������ �Լ�

int main(int argc, char **argv) {

	WSADATA wsaData;
	SOCKET servSock, clntSock;
	SOCKADDR_IN servAddr, clntAddr;

	char message[BUFSIZE];
	int strLen;
	int fromLen, nRcv;

	if (argc != 2) {
		printf("Please, Insert Port Number\n");
		exit(1);		//���α׷� ���� ��, ��Ʈ�ѹ��� ���� �Է����� �ʾ��� ���
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)		//���μ������� Winsock DLL�� ����� �ʱ�ȭ
		ErrorHandling("Load WinSock 2.2 DLL Error");	//������ ��� ErrorHandling

	servSock = socket(PF_INET, SOCK_STREAM, 0);		//server socket ����
	if (servSock == INVALID_SOCKET)
		ErrorHandling("Socket Error");		//������ ��� ErrorHandling

	memset(&servAddr, 0, sizeof(SOCKADDR_IN));
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(atoi(argv[1]));
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);		//TCP/IP ������ ���� �ʱ� ����

	if (bind(servSock, (struct sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)		//bind
		ErrorHandling("Bind Error");

	if (listen(servSock, 2) == SOCKET_ERROR)		//listen
		ErrorHandling("Listen Error");

	fromLen = sizeof(clntAddr);		//client �ּ� ���� ����

	clntSock = accept(servSock, (struct sockaddr*)&clntAddr, &fromLen);			//client socket accept
	if (clntSock == INVALID_SOCKET) {
		ErrorHandling("Accept Error");
	}
	else {
		printf("%s Connection Complete!\n", inet_ntoa(clntAddr.sin_addr));
		printf("Start...\n");
	}		//���� ����

	closesocket(servSock);		//server socket close


	while (1) {		//��ȣ �˻縦 ������ while��
		nRcv = recv(clntSock, message, sizeof(message) - 1, 0);		//client�κ��� �Է¹��� ��ȣ ����
		if (nRcv == SOCKET_ERROR) {
			printf("Receive Error..\n");
		}
		message[nRcv] = '\0';		//message�� ������ ���ڿ� NULL
		if (strcmp(message, PASSWORD) == 0) {		//client�� �Է��� ��ȣ�� ������ ��ȣ�� ��ġ�Ѵٸ� �α��� ����
			printf("Login Success");
			send(clntSock, "OK", (int)strlen("OK"), 0);		//client���� �α����� �����ߴٰ� ����
			break;		//��ȣ �˻� �Ϸ�, while�� Ż��
		}
		else {
			printf("Login Fail!\n");
			send(clntSock, "notOK", (int)strlen("notOK"), 0);		//��ȣ�� Ʋ�ȴٸ� �α��� ����, client���� �α����� �����ߴٰ� ���� ��, while�� �ݺ�
		}
	}
	

	while (1) {
		nRcv = recv(clntSock, message, sizeof(message) - 1, 0);		//client�κ��� command ����

		if (nRcv == SOCKET_ERROR) {
			printf("Receive Error..\n");
			break;
		}
		message[nRcv] = '\0';

		if (strcmp(message, "exit") == 0) {
			printf("Close Client Connection..\n");		//client�� exit�� �����ߴٸ� while���� Ż���Ͽ� ���α׷� ����
			break;
		}
		system(message);		//command ����
		printf("--------------------------------------");
		printf(" Command : [ %s ] Success ", message);
		printf("--------------------------------------\n");		//������ ��ɾ �������� ���
	}

	closesocket(clntSock);		//client socket close
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
