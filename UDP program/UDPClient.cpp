#include <stdio.h>
#include <WinSock2.h>		//socket programming을 하기 위한 library include

#define MAX 1024		//buffer size

int main(int argc, char **argv)
{
	WSADATA wsaData;
	SOCKET hServSock;
	struct sockaddr_in servAddr;
	char buf1[MAX] = "\0";
	char buf2[MAX] = "\0";
	int slen;

	char sAddr[15];		//server 주소
	int sPort;			//port number
	char message[MAX];	//입력을 받을 변수

	printf("Server Address : ");
	gets_s(sAddr);			//server 주소를 입력받음
	printf("Port Number : ");
	gets_s(message);		//port number를 message에 입력받기
	sPort = atoi(message);	//문자열 message을 정수로 변환하여 sPort에 저장


	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)		//프로세스에서 Winsock DLL의 사용을 초기화
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

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(sAddr);
	servAddr.sin_port = htons(sPort);		//UDP/IP 통신을 하기 위한 초기 설정


	while (1) {
		printf("동물을 선택하세요\n");
		printf("1. 고양이    2. 돼지    3. 우는 토끼\n");
		printf("선택 : ");		//선택 목록 출력
		gets_s(buf1, MAX);		//buf1에 입력받은 값 저장
		sendto(hServSock, buf1, MAX, 0, (struct sockaddr *) &servAddr, sizeof(servAddr));		//server로 선택항목값 전송
		slen = sizeof(servAddr);	//서버주소 사이즈 저장
		recvfrom(hServSock, buf1, MAX, 0, (struct sockaddr *) &servAddr, &slen);		//server로부터 첫번째 프레임 수신
		recvfrom(hServSock, buf2, MAX, 0, (struct sockaddr *) &servAddr, &slen);		//server로부터 두번째 프레임 수신
		
		for (int i = 0; i < 10; i++) {		//애니메이션 10번 반복
			system("cls");		//화면 초기화
			printf("%s\n", buf1);		//첫번째 프레임 출력
			system("cls");		//화면 초기화
			printf("%s\n", buf2);		//두번째 프레임 출력

		}

	}

}
