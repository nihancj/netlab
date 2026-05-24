#include <netinet/in.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main() {
	int sockid, nsockid; unsigned int cli_len;
	char recvline[1024] = {0}, sendline[1024] = {0};
	struct sockaddr_in ser_adr, cli_adr;

	sockid = socket(AF_INET, SOCK_STREAM, 0);
	if(sockid < 0) {
		printf("Socket creation failed");
		goto stop;
	}
	
	ser_adr.sin_family = AF_INET;
	ser_adr.sin_port = htons(3000);
	ser_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	int b = bind(sockid, (struct sockaddr *)&ser_adr, sizeof(ser_adr));
	if(b < 0) {
		printf("Bind failed");
		goto stop;
	}

	listen(sockid, 0);

	nsockid = accept(sockid, (struct sockaddr *)&cli_adr, &cli_len);
	printf("Client connected\n");

	do {
		read(nsockid, recvline, sizeof(recvline));
		printf("Client: %s\n", recvline);

		printf("Server: ");
		scanf("%s", sendline);
		write(nsockid, sendline, strlen(sendline)+1);

	}while (strncmp(sendline, "exit", 4) || strncmp(recvline, "exit", 4) );

stop:
	close(nsockid);
	close(sockid);
	return 0;
}
