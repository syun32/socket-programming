#include <stdio.h>
#include <WinSock2.h>		//socket programming을 하기 위한 library include
#include "emoji.h"			//프레임들을 저장한 헤더파일 include

#define MAX 1024		//buffer size

int main(int argc, char **argv)
{
	WSADATA wsaData;
	SOCKET hServSock;
	struct sockaddr_in hServAddr, hClntAddr;
	char buf[MAX] = "\0";
	int clen = sizeof(hClntAddr);

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) 		//프로세스에서 Winsock DLL의 사용을 초기화
	{
		fprintf(stderr, "WSAStartup() failed");
		exit(1);			//실패할 경우
	}

	if ((hServSock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)		//UDP/IP socket 생성
	{
		fprintf(stderr, "socket() failed");
		WSACleanup();
		exit(1);			//실패한 경우 winsock 사용을 종료
	}

	memset(&hServAddr, 0, sizeof(hServAddr));
	hServAddr.sin_family = AF_INET;
	hServAddr.sin_addr.s_addr = INADDR_ANY;
	hServAddr.sin_port = htons(atoi(argv[1]));		//UDP/IP 통신을 하기 위한 초기 설정

	if (bind(hServSock, (struct sockaddr *)&hServAddr, sizeof(hServAddr)) == SOCKET_ERROR)		//bind
	{
		printf("bind() error!\n");
		WSACleanup();
		closesocket(hServSock);
		exit(1);		//Error 발생시 winsock 사용 종류, 소켓 삭제
	}
	printf("bind() ok..\n");	//bind 성공 메시지 출력

	while (1) {
		recvfrom(hServSock, buf, MAX, 0, (struct sockaddr *) &hClntAddr, &clen);		//client로부터 선택 항목 값 수신하여 buf에 저장
		printf("%s 수신 ->  ", buf);		//선택 항목 값 출력

		if (strcmp(buf, "1") == 0) {
			sendto(hServSock, cat1, MAX, 0, (struct sockaddr *) &hClntAddr, sizeof(hClntAddr));
			sendto(hServSock, cat2, MAX, 0, (struct sockaddr *) &hClntAddr, sizeof(hClntAddr));
			printf("고양이 보냄\n");
		}		//1번 항목 선택시, 고양이 프레임 client에게 전송
		else if (strcmp(buf, "2") == 0) {
			sendto(hServSock, pig1, MAX, 0, (struct sockaddr *) &hClntAddr, sizeof(hClntAddr));
			sendto(hServSock, pig2, MAX, 0, (struct sockaddr *) &hClntAddr, sizeof(hClntAddr));
			printf("돼지 보냄\n");
		}		//2번 항목 선택시, 돼지 프레임 client에게 전송
		else if (strcmp(buf, "3") == 0) {
			sendto(hServSock, rab1, MAX, 0, (struct sockaddr *) &hClntAddr, sizeof(hClntAddr));
			sendto(hServSock, rab2, MAX, 0, (struct sockaddr *) &hClntAddr, sizeof(hClntAddr));
			printf("우는 토끼 보냄\n");
		}		//3번 항목 선택시, 토끼 프레임 client에게 전송
		else if (strcmp(buf, "exit") == 0) {
			printf("ByeBye!");
			break;
		}		//client에서 exit 전송시, 메시지 출력 후 종료
		
	}

}
