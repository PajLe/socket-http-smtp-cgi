#include <winsock.h>
#pragma comment(lib, "wsock32.lib")

#include <string.h>
#include <string>
#include <iostream>
#include <fstream>

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
	strtok_s(recv_data, " ", &nextToken);
	char* location = strtok_s(NULL, " ", &nextToken);

	std::string headerBad = "HTTP/1.1 404 not found\r\n\r\n";

	std::string headerGood = "HTTP/1.1 200 OK\r\nContent-Type:text/html\r\n\r\n";
	//std::string bodyGood = "<h1>My Home page</h1>";

	
	std::ifstream file(location + 1, std::ios::binary | std::ios::ate);
	int fileSize = file.tellg();
	file.close();

	std::ifstream myfile(location + 1);
	if (strstr(lineAccept, "text/html") != NULL && myfile.is_open()) {
		std::string line;
		headerGood.append("Content-Length:");
		headerGood.append(std::to_string(fileSize));
		headerGood.append("\r\n\r\n");
		while (std::getline(myfile, line)) {
			headerGood.append(line); 
			//headerGood.append("\r\n");
		}
		myfile.close();
		send(acceptSock, headerGood.c_str(), headerGood.size(), 0);
	}
	else
		send(acceptSock, headerBad.c_str(), headerBad.size(), 0);

	closesocket(listensock);
	WSACleanup();

}
