#include <netinet/in.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
	int sockid, nsockid;
	unsigned int cli_len;
	char recvline[1024], sendline[1024];
	struct sockaddr_in ser_adr, cli_adr;

	sockid = socket(AF_INET, SOCK_STREAM, 0);
	if(sockid < 0) {
		printf("Socket creation failed");
		goto stop;
	}
	
	memset(&ser_adr, 0, sizeof(ser_adr));
	memset(&cli_adr, 0, sizeof(cli_adr));
	ser_adr.sin_family = AF_INET;
	ser_adr.sin_port = htons(3000);
	ser_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	int b = connect(sockid, (struct sockaddr *)&ser_adr, sizeof(ser_adr));
	if(b < 0) {
		printf("Bind failed\n");
		goto stop;
	}
	memset(recvline, 0, sizeof(recvline));
	memset(sendline, 0, sizeof(sendline));

	do {
		printf("Client: ");
		scanf("%s", sendline);
		write(sockid, sendline, strlen(sendline)+1);

		read(sockid, recvline, sizeof(recvline));
		printf("Server: %s\n", recvline);

	}while (strncmp(sendline, "exit", 4) || strncmp(recvline, "exit", 4));

stop:
	close(nsockid);
	close(sockid);
	return 0;
}
