#include <winsock.h>
#pragma comment(lib, "wsock32.lib")

#include <string.h>
#include <iostream>

int main() {
	WSAData wsa;
	WSAStartup(0x0202, &wsa);

	SOCKET sock;
	sockaddr_in server_addr;

	char header[1024] = "GET /index.html HTTP/1.1\r\n";
	strcat(header, "Accept: text/html\r\n\r\n");
	char recv_data[1024];

	sock = socket(AF_INET, SOCK_STREAM, 0);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(80);
	server_addr.sin_addr.S_un.S_addr = inet_addr("198.162.12.118");

	connect(sock, (sockaddr*)&server_addr, sizeof(sockaddr));
	send(sock, header, strlen(header) + 1, 0);
	recv(sock, recv_data, 1024, 0);

	if (strstr(recv_data, "OK") != NULL) 
		std::cout << "fajl postoji";
	else
		std::cout << "fajl ne postoji";

	closesocket(sock);
	WSACleanup();
}
