#include <winsock.h>
#pragma comment(lib, "wsock32.lib")

#include <string.h>
#include <iostream>

int main() {
	WSAData wsa;
	WSAStartup(0x0202, &wsa);

	SOCKET sock;
	sockaddr_in server_addr;

	char header[] = "GET / HTTP/1.1\r\n\r\n";
	char recv_data[1024];

	sock = socket(AF_INET, SOCK_STREAM, 0);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(80);
	server_addr.sin_addr.S_un.S_addr = inet_addr("216.58.212.4"); // google

	connect(sock, (sockaddr*)&server_addr, sizeof(sockaddr));
	send(sock, header, strlen(header) + 1, 0);
	recv(sock, recv_data, 1024, 0);

	char* serverSoftware = strstr(recv_data, "Server:");
	char* nextToken = nullptr;
	
	std::cout << strtok_s(serverSoftware, "\r\n", &nextToken) << std::endl;

	closesocket(sock);
	WSACleanup();
}