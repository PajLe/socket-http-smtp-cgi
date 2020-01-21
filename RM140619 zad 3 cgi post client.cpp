#include <winsock.h>
#pragma comment(lib, "wsock32.lib")

#include <string.h>
#include <stdio.h>

bool sendPOST(int broj1, int broj2, char znak) {
	WSAData wsa;
	WSAStartup(0x0202, &wsa);

	sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(80);
	server_addr.sin_addr.S_un.S_addr = inet_addr("215.20.17.3");

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	connect(sock, (sockaddr*)&server_addr, sizeof(sockaddr));

	char send_data[1024] = "POST /cgi-bin/Calculator.exe HTTP/1.1\r\n\r\n";

	char broj[10];
	strcat(send_data, "broj1=");
	sprintf(broj, "%d", broj1);
	strcat(send_data, broj);

	strcat(send_data, "&broj2=");
	sprintf(broj, "%d", broj2);
	strcat(send_data, broj);

	strcat(send_data, "&znak=");
	sprintf(broj, "%c", znak);
	strcat(send_data, broj);

	send(sock, send_data, strlen(send_data) + 1, 0);

	char recv_data[1024];
	recv(sock, recv_data, 1024, 0);

	if (strstr(recv_data, "OK")) {
		strtok(recv_data, "\r\n\r\n");
		printf("%s", strtok(NULL, "\n"));
		closesocket(sock);
		WSACleanup();
		return true;
	}
	
	closesocket(sock);
	WSACleanup();
	return false;
}
