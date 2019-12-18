#include <winsock.h>
#pragma comment(lib, "wsock32.lib")

#include <string.h>
#include <stdio.h>

int main() {
	WSAData wsa;
	WSAStartup(0x0202, &wsa);

	SOCKET listensock;
	sockaddr_in server_addr;

	char recv_data[1024];

	listensock = socket(AF_INET, SOCK_STREAM, 0);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(80);
	server_addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	bind(listensock, (sockaddr*)&server_addr, sizeof(sockaddr));
	listen(listensock, 2);

	SOCKET acceptSock;
	do {
		acceptSock = accept(listensock, nullptr, nullptr);
	} while (acceptSock == INVALID_SOCKET);


	recv(acceptSock, recv_data, 1024, 0);

	char* nextToken = NULL;
	char* lineAccept = strtok_s(strstr(recv_data, "Accept: "), "\r\n", &nextToken);
	strtok_s(recv_data, " ", &nextToken);
	char* location = strtok_s(NULL, " ", &nextToken);
	const char* headerBad = "HTTP/1.1 404 not found\r\n\r\n";

	int fileLength = 0;
	char* baferZaSlanje = NULL;
	FILE* f = fopen(location + 1, "rb"); // location = "/file.png"; location + 1 = "file.png"
	if (f) {
		fseek(f, 0, SEEK_END);
		fileLength = ftell(f);
		baferZaSlanje = (char*)malloc(fileLength + 150);
		strcpy(baferZaSlanje, "HTTP/1.1 ");
		strcat(baferZaSlanje, "200 OK \r\n");
		strcat(baferZaSlanje, "Content-Length: ");
		char broj[10];
		sprintf(broj, "%d", fileLength);
		strcat(baferZaSlanje, broj);
		strcat(baferZaSlanje, "\r\n");
		strcat(baferZaSlanje, "Content-Type: image/jpeg\r\n");
		strcat(baferZaSlanje, "\r\n");

		fseek(f, 0, SEEK_SET);
		char* fajl = baferZaSlanje + strlen(baferZaSlanje); // fajl pokazuje na '\0' headera
		int brojac = 0;
		char karakter;
		fread(&karakter, sizeof(char), 1, f);
		while (!feof(f))
		{
			fajl[brojac++] = karakter;
			fread(&karakter, sizeof(char), 1, f);
		}

		int duzinaZaSlanje = strlen(baferZaSlanje) + fileLength;
		send(acceptSock, baferZaSlanje, duzinaZaSlanje, 0);

		fclose(f);
		delete baferZaSlanje;
	} else
		send(acceptSock, headerBad, strlen(headerBad) + 1, 0);

	closesocket(listensock);
	WSACleanup();
}