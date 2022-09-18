#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<winsock2.h>

#pragma comment(lib,"ws2_32.lib")

#define BUF_SIZE 1024
void ErrorHandling(const char* message);

int main(int argc, char* argv[]) {
	WSADATA wsaData;
	SOCKET hSocket;
	char message[BUF_SIZE];
	int sendLen, recvLen, recvCount;
	SOCKADDR_IN servAdr;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servAdr.sin_port = htons(5000);

	if (connect(hSocket, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
		ErrorHandling("connect() error!");
	else
		puts("Connected...........");

	while (1) {
		fputs("Input message(Q to quit): ", stdout);
		fgets(message, BUF_SIZE, stdin);

		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;
		sendLen = send(hSocket, message, strlen(message), 0);

		recvLen = 0;
		while (sendLen > recvLen) {
			recvCount = recv(hSocket, message, BUF_SIZE - 1, 0);
			recvLen += recvCount;
		}
		message[sendLen] = 0;
		printf("Message from server: %s", message);
	}
	closesocket(hSocket);
	WSACleanup();
	return 0;

}

void ErrorHandling(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}