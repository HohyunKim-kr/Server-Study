#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")
#define BUFSIZE 1024

void ErrorHandling(char* message);

int main(int argc, char** argv) {

	WSADATA wsaData;
	SOCKET serverSocket, clientSocket;
	SOCKADDR_IN serverAddress, clientAddress;

	int stringLength;
	char message[BUFSIZE] = { 0, };
	char data[BUFSIZE] = { 0, };
	int clientAddressSize;

	if (argc != 2) {
		printf("Usage %s <port> \n", argv[0]);
		exit(1);
	}

	//소켓 라이브러리 초기화
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		ErrorHandling("WSAStartup() Error!");
	}


	//소켓 생성
	serverSocket = socket(PF_INET, SOCK_STREAM, 0);
	//소켓이 유효하지 않을 경우
	if (serverSocket == INVALID_SOCKET) {
		ErrorHandling("socket() Error!");
	}

	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddress.sin_port = htons(atoi(argv[1]));

	//IP주소와 포트번호 할당, binding
	if (bind(serverSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
		ErrorHandling("bind() Error!");
	}

	//listen 함수를 통해 서버 소켓 완성
	if (listen(serverSocket, 5) == SOCKET_ERROR) {
		ErrorHandling("listen() Error!");
	}

	clientAddressSize = sizeof(clientAddress);


	while (1) {

		//클라이언트의 연결요청을 수락, accept 
		clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddress, &clientAddressSize);

		if (clientSocket == -1) {
			ErrorHandling("accept() Error!");
		}
		else {
			printf("Accept client \n");
		}

		stringLength = strlen(message);

		while ((stringLength = recv(clientSocket, message, BUFSIZE, 0)) != 0) {

			if (message == 13) {
				printf("disconnect client \n");
				break;
			}

			if (stringLength == -1) {
				printf("disconnect client \n");
				break;
			}

			memset(data, 0, BUFSIZE);
			memcpy(data, message, stringLength);
			printf("data : %s , length : %d \n", message, stringLength);
			send(clientSocket, message, stringLength, 0);
		}

		closesocket(clientSocket);
		closesocket(serverSocket);
		//소켓 라이브러리 해제
		WSACleanup();
		return 0;

	}

}

void ErrorHandling(char* message) {

	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);

}