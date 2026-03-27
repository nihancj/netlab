#include <netinet/in.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main() {
	int sockid;
	struct sockaddr_in ser_adr, cli_adr;
	char recvline[1024], sendline[1024];

	sockid = socket(AF_INET, SOCK_DGRAM, 0);

	ser_adr.sin_family = AF_INET;
	ser_adr.sin_port = htons(3001);
	ser_adr.sin_addr.s_addr = htonl(INADDR_ANY);

	int b = bind(sockid, (struct sockaddr *)&ser_adr, sizeof(ser_adr));
	if (b < 0) {
		printf("Bind failed\n");
	}
	memset(sendline, 0, sizeof(sendline));
	memset(recvline, 0, sizeof(recvline));

	unsigned int cli_len = sizeof(cli_adr);
	do {
		int n = recvfrom(sockid, recvline, sizeof(recvline), 0, (struct sockaddr *)&cli_adr, &cli_len);
		recvline[n] = '\0';
		printf("Client: %s\n", recvline);

		printf("Server: ");
		scanf("%s", sendline);
		sendto(sockid, sendline, strlen(sendline)+1, 0, (struct sockaddr *)&cli_adr, cli_len);
	}while (strncmp(sendline, "exit", 4) || strncmp(recvline, "exit", 4) );

	return 0;
}
