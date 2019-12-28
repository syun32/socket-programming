#include <WinSock2.h>		//socket programming을 하기 위한 library include
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#define BUFSIZE 1024

void ErrorHandling(const char *message);	//Error가 발생했을 때 실행할 함수

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
	sPort = atoi(message);		//server address, port number를 입력받음

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)		//프로세스에서 Winsock DLL의 사용을 초기화
		ErrorHandling("Load WinSock 2.2 DLL Error");	//실패할 경우 ErrorHandling

	clntSock = socket(AF_INET, SOCK_STREAM, 0);		//client socket 생성
	if (clntSock == INVALID_SOCKET)
		ErrorHandling("Socket Error");			//실패할 경우 ErrorHandling

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(sAddr);
	servAddr.sin_port = htons(sPort);		//TCP/IP 통신을 하기 위한 초기 설정

	if (connect(clntSock, (struct sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) {		//connect
		ErrorHandling("Connection Error");
	}
	else {
		printf("Connect OK!\nStart...\n");
	}

	while (1) {		//암호 검사 while문
		printf("\nPassword : ");
		gets_s(message);		//암호를 입력한다


		send(clntSock, message, (int)strlen(message), 0);		//server에 입력한 암호 전송
		nRcv = recv(clntSock, message, sizeof(message) - 1, 0);		//server로부터 암호 검사 결과 수신
		if (nRcv == SOCKET_ERROR) {
			printf("Receive Error..\n");
			break;		//수신 오류가 있다면 종료
		}
		message[nRcv] = '\0';		//message 마지막 문자에 NULL

		if (strcmp(message, "OK") == 0) {		//암호가 옳다는 의미의 OK도착 시
			printf("Login Success\n");
			break;			//로그인에 성공하여 암호검사 while문 탈출
		}
		else
			printf("Login Fail\n");		//암호 불일치시 암호검사 while문 반복
	}

	while (1) {
		printf("\nSend Command : ");
		gets_s(message);		//전송할 command 입력

		if (strcmp(message, "exit") == 0) {		//exit 입력 시, 서버에 전송 후 종료
			send(clntSock, message, (int)strlen(message), 0);
			break;
		}

		send(clntSock, message, (int)strlen(message), 0);	//입력한 command server로 전송

	}

		closesocket(clntSock);		//client socket delete
		WSACleanup();		//winsock 사용을 종료
		printf("Close Connection..\n");
		_getch();

		return 0;
	}


void ErrorHandling(const char *message) {	//Error발생 시 winsock 사용을 종료하고 메시지를 출력할 함수
	WSACleanup();		
	fputs(message, stderr);
	fputc('\n', stderr);
	_getch();
	exit(1);
}