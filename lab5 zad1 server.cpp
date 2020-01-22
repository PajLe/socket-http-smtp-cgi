#include <winsock.h>
#pragma comment(lib, "wsock32.lib")

#include <string.h>
#include <iostream>

int main() {
	WSAData wsa;
	WSAStartup(0x0202, &wsa);

	SOCKET listensock;
	sockaddr_in server_addr;
	
	char recv_data[1024];

	listensock = socket(AF_INET, SOCK_STREAM, 0);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(80);
	server_addr.sin_addr.S_un.S_addr = inet_addr("192.168.0.17");

	bind(listensock, (sockaddr*)&server_addr, sizeof(sockaddr));
	listen(listensock, 1);

	SOCKET acceptSock;
	do {
		acceptSock = accept(listensock, nullptr, nullptr);
	} while (acceptSock == INVALID_SOCKET);
		

	recv(acceptSock, recv_data, 1024, 0);

	char* nextToken = NULL;
	char* lineAccept = strtok_s(strstr(recv_data, "Accept: "), "\r\n", &nextToken);

	char headerBad[] = "HTTP/1.1 400 bad request\r\n\r\n";
	char headerGood[] = "HTTP/1.1 200 OK\r\n\r\n";

	if (strstr(lineAccept, "image/gif") != NULL
			|| strstr(lineAccept, "image/png") != NULL
			|| strstr(lineAccept, "image/jpeg") != NULL) {
		send(acceptSock, headerGood, strlen(headerGood) + 1, 0);
	}
	else
		send(acceptSock, headerBad, strlen(headerBad) + 1, 0);
	
	closesocket(listensock);
	WSACleanup();
}
