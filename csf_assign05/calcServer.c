/*
 * Implementation of the calculator server
 * CSF Assignment 5
 * Aidan Aug, Will Zhao
 * aaug1@jhu.edu, wzhao33@jhu.edu
 */

#include <stdio.h>      /* for snprintf */
#include "csapp.h"
#include "calc.h"

/* buffer size for reading lines of input from user */
#define LINEBUF_SIZE 1024



int chat_with_client(struct Calc *calc, int infd, int outfd);
int accept_connection(int ssock_fd, struct sockaddr_in *clientaddr);
int create_server_socket(int port);


int main(int argc, char **argv) {
	/* TODO: implement this program */
	char buf[256];
	int port = atoi(argv[1]);
	int ssock_fd = create_server_socket(port);
	struct Calc *calc = calc_create();

	while (1) {
		struct sockaddr_in clientaddr;
		int clientfd = accept_connection(ssock_fd, &clientaddr);
		int shutdown = chat_with_client(calc, clientfd, clientfd);
			//write(clientfd, buf, rc);
		if (shutdown == 0) {
			return 0;
		} else if(shutdown == 1) {
			close(clientfd);
		}

	}
}


int chat_with_client(struct Calc *calc, int infd, int outfd) {
	rio_t in;
	char linebuf[LINEBUF_SIZE];

	/* wrap standard input (which is file descriptor 0) */
	rio_readinitb(&in, infd);

	/*
	 * Read lines of input, evaluate them as calculator expressions,
	 * and (if evaluation was successful) print the result of each
	 * expression.  Quit when "quit" command is received.
	 */
	int done = 0;
	while (!done) {
		ssize_t n = rio_readlineb(&in, linebuf, LINEBUF_SIZE);
		if (n <= 0) {
			/* error or end of input */
			done = 1;
		} else if (strcmp(linebuf, "quit\n") == 0 || strcmp(linebuf, "quit\r\n") == 0) {
			/* quit command */
			done = 1;
		} else if (strcmp(linebuf, "shutdown\n") == 0 || strcmp(linebuf, "shutdown\r\n") == 0) {
			return 0;
		} else {
			/* process input line */
			int result;
			if (calc_eval(calc, linebuf, &result) == 0) {
				/* expression couldn't be evaluated */
				rio_writen(outfd, "Error\n", 6);
			} else {
				/* output result */
				int len = snprintf(linebuf, LINEBUF_SIZE, "%d\n", result);
				if (len < LINEBUF_SIZE) {
					rio_writen(outfd, linebuf, len);
				}
			}
		}
	}
	return 1;
}

int accept_connection(int ssock_fd, struct sockaddr_in *clientaddr) {
	unsigned clientlen = sizeof(clientaddr);
	int childfd = accept(ssock_fd, (struct sockaddr *) clientaddr, &clientlen);

	if (childfd < 0) {
		printf("accept failed");
		exit(1);

	}
	return childfd;
}

int create_server_socket(int port) {
	struct sockaddr_in serveraddr = {0};
	int ssock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (ssock_fd < 0) {
		printf("accept failed");
		exit(1);

	}

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons((unsigned short)port);
	if (bind(ssock_fd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0) {
		printf("bind failed");
		exit(1);
	}

	if (listen(ssock_fd, 5) < 0) {
		printf("listen failed");
		exit(1);
	}

	return ssock_fd;
}
