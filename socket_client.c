#include <stdio.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <fcntl.h>

#define MAXBUF 1024

int main(int argc, char **argv){
	int sock_cli;
	struct sockaddr_in cliaddr;
	char buf[MAXBUF];
	int send_re;
	int open_re;
	int read_len;

	if(argc < 4){
		puts("Usage : Command [IP] [PORT] [FILE]");
		return -1;
	}
	
	if((sock_cli = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		puts("Socket error");
		return -1;
	}

	cliaddr.sin_family = AF_INET;
	//cliaddr.sin_addr.s_addr = inet_addr(argv[1]);
	inet_pton(AF_INET, argv[1], &cliaddr.sin_addr);
	cliaddr.sin_port = htons(atoi(argv[2]));

	if(connect(sock_cli, (struct sockaddr*)&cliaddr, sizeof(cliaddr)) == -1){
		puts("Connection error");
		close(sock_cli);
		return -1;
	}
	puts("Connection Success");

	send_re = send(sock_cli, argv[3], MAXBUF, 0);
	if(send_re == -1){
		puts("Send error");
		return -1;
	}

	open_re = open(argv[3], O_RDONLY);
	if(open_re == -1){
		puts("Open error");
		close(open_re);
		return -1;
	}

	while(1){
		memset(buf, 0x00, MAXBUF);
		read_len = read(open_re, buf, MAXBUF);
		send(sock_cli, buf, read_len, 0);
		if(read_len == 0){
			puts("Transfer complete");
			break;
		}
		fflush(stdout);
	}
	close(sock_cli);
	close(open_re);

	return 0;
}
