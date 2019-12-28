#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>		//socket programming을 하기 위한 library include
#include <conio.h>


#define BUFSIZE 1024
#define PASSWORD "맞는 비밀번호"		//비밀번호 설정

void ErrorHandling(const char *message);	//Error가 발생했을 때 실행할 함수

int main(int argc, char **argv) {

	WSADATA wsaData;
	SOCKET servSock, clntSock;
	SOCKADDR_IN servAddr, clntAddr;

	char message[BUFSIZE];
	int strLen;
	int fromLen, nRcv;

	if (argc != 2) {
		printf("Please, Insert Port Number\n");
		exit(1);		//프로그램 실행 시, 포트넘버를 같이 입력하지 않았을 경우
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)		//프로세스에서 Winsock DLL의 사용을 초기화
		ErrorHandling("Load WinSock 2.2 DLL Error");	//실패할 경우 ErrorHandling

	servSock = socket(PF_INET, SOCK_STREAM, 0);		//server socket 생성
	if (servSock == INVALID_SOCKET)
		ErrorHandling("Socket Error");		//실패할 경우 ErrorHandling

	memset(&servAddr, 0, sizeof(SOCKADDR_IN));
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(atoi(argv[1]));
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);		//TCP/IP 연결을 위한 초기 설정

	if (bind(servSock, (struct sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)		//bind
		ErrorHandling("Bind Error");

	if (listen(servSock, 2) == SOCKET_ERROR)		//listen
		ErrorHandling("Listen Error");

	fromLen = sizeof(clntAddr);		//client 주소 길이 저장

	clntSock = accept(servSock, (struct sockaddr*)&clntAddr, &fromLen);			//client socket accept
	if (clntSock == INVALID_SOCKET) {
		ErrorHandling("Accept Error");
	}
	else {
		printf("%s Connection Complete!\n", inet_ntoa(clntAddr.sin_addr));
		printf("Start...\n");
	}		//연결 성공

	closesocket(servSock);		//server socket close


	while (1) {		//암호 검사를 수행할 while문
		nRcv = recv(clntSock, message, sizeof(message) - 1, 0);		//client로부터 입력받은 암호 수신
		if (nRcv == SOCKET_ERROR) {
			printf("Receive Error..\n");
		}
		message[nRcv] = '\0';		//message의 마지막 문자에 NULL
		if (strcmp(message, PASSWORD) == 0) {		//client가 입력한 암호가 설정된 암호와 일치한다면 로그인 성공
			printf("Login Success");
			send(clntSock, "OK", (int)strlen("OK"), 0);		//client에게 로그인을 성공했다고 전송
			break;		//암호 검사 완료, while문 탈출
		}
		else {
			printf("Login Fail!\n");
			send(clntSock, "notOK", (int)strlen("notOK"), 0);		//암호가 틀렸다면 로그인 실패, client에게 로그인을 실패했다고 전송 후, while문 반복
		}
	}
	

	while (1) {
		nRcv = recv(clntSock, message, sizeof(message) - 1, 0);		//client로부터 command 수신

		if (nRcv == SOCKET_ERROR) {
			printf("Receive Error..\n");
			break;
		}
		message[nRcv] = '\0';

		if (strcmp(message, "exit") == 0) {
			printf("Close Client Connection..\n");		//client가 exit를 전송했다면 while문을 탈출하여 프로그램 종료
			break;
		}
		system(message);		//command 수행
		printf("--------------------------------------");
		printf(" Command : [ %s ] Success ", message);
		printf("--------------------------------------\n");		//실행한 명령어가 무엇인지 출력
	}

	closesocket(clntSock);		//client socket close
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
