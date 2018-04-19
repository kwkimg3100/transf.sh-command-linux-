#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#define MAXBUF 1024

int main(int argc, char **argv){
	struct sockaddr_in sockaddr, cli_sock;
	int sockde;
	int acc_sock;
	socklen_t cli_sock_size;
	char buf[MAXBUF];
	int read_len;
	int open_re;
	int file_read_len;
	
	if(argc < 2){
		puts("Usage : command [port]");
		return -1;
	}

	if((sockde = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		puts("Socket generating error");
		return -1;
	}

	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(atoi(argv[1]));
	sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(sockde, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0){
		puts("Bind error");
		return -1;
	}

	listen(sockde, 2);
	
	cli_sock_size = sizeof(cli_sock);

	while(1){
		char file_name[MAXBUF];

		puts("Listening...");
		acc_sock = accept(sockde, (struct sockaddr*)&cli_sock, &cli_sock_size);
		if(acc_sock == -1){
			puts("Accept error");
			return -1;
		}
		else{	
			printf("Client %s connect\n",inet_ntoa(cli_sock.sin_addr));
		}
		
		read_len = read(acc_sock, buf, MAXBUF);
		if(read_len > 0){
			strcpy(file_name, buf);
			printf("%s -> %s\n", inet_ntoa(cli_sock.sin_addr), file_name);
			fflush(stdout);
		}
		else{
			close(acc_sock);
			puts("File name error");
			return -1;
		}
		
		open_re = open(file_name, O_WRONLY | O_CREAT | O_EXCL, 0644);
		if(open_re == -1){
			puts("Open error");
			return -1;
		}
		while(1){
			memset(buf, 0x00, MAXBUF);
			file_read_len = read(acc_sock, buf, MAXBUF);
			write(open_re, buf, file_read_len);

			if(file_read_len == EOF | file_read_len == 0){
				puts("Transfer file finish");
				break;
			}
		
		}
	}

	close(open_re);
	close(acc_sock);
	close(sockde);
	
	return 0;
}
