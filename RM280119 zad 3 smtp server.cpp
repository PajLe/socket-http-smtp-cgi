#include <winsock.h>
#pragma comment(lib, "wsock32.lib")

#include <string.h>

#define KB 1024

bool primiSMTP() {
	WSAData wsa;
	WSAStartup(0x0202, &wsa);

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(25);
	addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	SOCKET listenSock = socket(AF_INET, SOCK_STREAM, 0);
	bind(listenSock, (sockaddr*)&addr, sizeof(sockaddr));
	listen(listenSock, 1);

	SOCKET acceptSock = accept(listenSock, NULL, NULL);

	char recv_data[KB], send_data[KB];

	strcpy(send_data, "220 elfak.rs");
	send(acceptSock, send_data, strlen(send_data) + 1, 0);
	recv(acceptSock, recv_data, KB, 0);

	if (strstr(recv_data, "HELO") != NULL) {
		strcpy(send_data, "250 Hello ");
		strtok(recv_data, " ");
		strcat(send_data, strtok(NULL, " "));
		strcat(send_data, ", pleased to meet you");

		send(acceptSock, send_data, strlen(send_data) + 1, 0);
		recv(acceptSock, recv_data, KB, 0);
		if (strstr(recv_data, "MAIL FROM:") != NULL) {
			strcpy(send_data, "250 ");
			strtok(recv_data, ":");
			strcat(send_data, strtok(NULL, ":"));
			strcat(send_data, " ... sender OK");

			send(acceptSock, send_data, strlen(send_data) + 1, 0);
			recv(acceptSock, recv_data, KB, 0);

			if (strstr(recv_data, "RCPT TO:") != NULL) {
				strcpy(send_data, "250 ");
				strtok(recv_data, ":");
				strcat(send_data, strtok(NULL, ":"));
				strcat(send_data, " ... recipient OK");

				send(acceptSock, send_data, strlen(send_data) + 1, 0);
				recv(acceptSock, recv_data, KB, 0);

				if (strstr(recv_data, "DATA") != NULL) {
					strcpy(send_data, "324 Enter mail, end with \".\" on a line by itself ");

					send(acceptSock, send_data, strlen(send_data) + 1, 0);
					recv(acceptSock, recv_data, KB, 0);

					if (strstr(recv_data, ".") != NULL) {
						strcpy(send_data, "250 message accepted for delivery");

						send(acceptSock, send_data, strlen(send_data) + 1, 0);
						recv(acceptSock, recv_data, KB, 0);

						if (strstr(recv_data, "QUIT") != NULL) {
							strcpy(send_data, "221 elfak.rs closing connection");

							send(acceptSock, send_data, strlen(send_data) + 1, 0);
						}
						else return false;
					}
					else return false;
				}
				else return false;
			}
			else return false;
		}
		else return false;
	}
	else return false;

	return true;
}
