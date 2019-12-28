#include <stdio.h>
#include <WinSock2.h>		//socket programming�� �ϱ� ���� library include
#include "emoji.h"			//�����ӵ��� ������ ������� include

#define MAX 1024		//buffer size

int main(int argc, char **argv)
{
	WSADATA wsaData;
	SOCKET hServSock;
	struct sockaddr_in hServAddr, hClntAddr;
	char buf[MAX] = "\0";
	int clen = sizeof(hClntAddr);

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) 		//���μ������� Winsock DLL�� ����� �ʱ�ȭ
	{
		fprintf(stderr, "WSAStartup() failed");
		exit(1);			//������ ���
	}

	if ((hServSock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)		//UDP/IP socket ����
	{
		fprintf(stderr, "socket() failed");
		WSACleanup();
		exit(1);			//������ ��� winsock ����� ����
	}

	memset(&hServAddr, 0, sizeof(hServAddr));
	hServAddr.sin_family = AF_INET;
	hServAddr.sin_addr.s_addr = INADDR_ANY;
	hServAddr.sin_port = htons(atoi(argv[1]));		//UDP/IP ����� �ϱ� ���� �ʱ� ����

	if (bind(hServSock, (struct sockaddr *)&hServAddr, sizeof(hServAddr)) == SOCKET_ERROR)		//bind
	{
		printf("bind() error!\n");
		WSACleanup();
		closesocket(hServSock);
		exit(1);		//Error �߻��� winsock ��� ����, ���� ����
	}
	printf("bind() ok..\n");	//bind ���� �޽��� ���

	while (1) {
		recvfrom(hServSock, buf, MAX, 0, (struct sockaddr *) &hClntAddr, &clen);		//client�κ��� ���� �׸� �� �����Ͽ� buf�� ����
		printf("%s ���� ->  ", buf);		//���� �׸� �� ���

		if (strcmp(buf, "1") == 0) {
			sendto(hServSock, cat1, MAX, 0, (struct sockaddr *) &hClntAddr, sizeof(hClntAddr));
			sendto(hServSock, cat2, MAX, 0, (struct sockaddr *) &hClntAddr, sizeof(hClntAddr));
			printf("����� ����\n");
		}		//1�� �׸� ���ý�, ����� ������ client���� ����
		else if (strcmp(buf, "2") == 0) {
			sendto(hServSock, pig1, MAX, 0, (struct sockaddr *) &hClntAddr, sizeof(hClntAddr));
			sendto(hServSock, pig2, MAX, 0, (struct sockaddr *) &hClntAddr, sizeof(hClntAddr));
			printf("���� ����\n");
		}		//2�� �׸� ���ý�, ���� ������ client���� ����
		else if (strcmp(buf, "3") == 0) {
			sendto(hServSock, rab1, MAX, 0, (struct sockaddr *) &hClntAddr, sizeof(hClntAddr));
			sendto(hServSock, rab2, MAX, 0, (struct sockaddr *) &hClntAddr, sizeof(hClntAddr));
			printf("��� �䳢 ����\n");
		}		//3�� �׸� ���ý�, �䳢 ������ client���� ����
		else if (strcmp(buf, "exit") == 0) {
			printf("ByeBye!");
			break;
		}		//client���� exit ���۽�, �޽��� ��� �� ����
		
	}

}
