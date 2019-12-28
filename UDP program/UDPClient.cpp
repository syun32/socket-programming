#include <stdio.h>
#include <WinSock2.h>		//socket programming�� �ϱ� ���� library include

#define MAX 1024		//buffer size

int main(int argc, char **argv)
{
	WSADATA wsaData;
	SOCKET hServSock;
	struct sockaddr_in servAddr;
	char buf1[MAX] = "\0";
	char buf2[MAX] = "\0";
	int slen;

	char sAddr[15];		//server �ּ�
	int sPort;			//port number
	char message[MAX];	//�Է��� ���� ����

	printf("Server Address : ");
	gets_s(sAddr);			//server �ּҸ� �Է¹���
	printf("Port Number : ");
	gets_s(message);		//port number�� message�� �Է¹ޱ�
	sPort = atoi(message);	//���ڿ� message�� ������ ��ȯ�Ͽ� sPort�� ����


	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)		//���μ������� Winsock DLL�� ����� �ʱ�ȭ
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

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(sAddr);
	servAddr.sin_port = htons(sPort);		//UDP/IP ����� �ϱ� ���� �ʱ� ����


	while (1) {
		printf("������ �����ϼ���\n");
		printf("1. �����    2. ����    3. ��� �䳢\n");
		printf("���� : ");		//���� ��� ���
		gets_s(buf1, MAX);		//buf1�� �Է¹��� �� ����
		sendto(hServSock, buf1, MAX, 0, (struct sockaddr *) &servAddr, sizeof(servAddr));		//server�� �����׸� ����
		slen = sizeof(servAddr);	//�����ּ� ������ ����
		recvfrom(hServSock, buf1, MAX, 0, (struct sockaddr *) &servAddr, &slen);		//server�κ��� ù��° ������ ����
		recvfrom(hServSock, buf2, MAX, 0, (struct sockaddr *) &servAddr, &slen);		//server�κ��� �ι�° ������ ����
		
		for (int i = 0; i < 10; i++) {		//�ִϸ��̼� 10�� �ݺ�
			system("cls");		//ȭ�� �ʱ�ȭ
			printf("%s\n", buf1);		//ù��° ������ ���
			system("cls");		//ȭ�� �ʱ�ȭ
			printf("%s\n", buf2);		//�ι�° ������ ���

		}

	}

}
