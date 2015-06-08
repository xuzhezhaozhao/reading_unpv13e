#include	"unp.h"
#include	<time.h>

int
main(int argc, char **argv)
{
	int					listenfd, connfd;
	struct sockaddr_in	servaddr;
	char				buff[MAXLINE];
	time_t				ticks;

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	/* 修改: 只 accept 127.0.0.1 的请求 */
	if (inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr) <= 0) {
		err_sys("inet_pton error");
	}
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(9999);	/* daytime server */

	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

	Listen(listenfd, LISTENQ);

	for ( ; ; ) {
		int i;
		connfd = Accept(listenfd, (SA *) NULL, NULL);

		printf("accept one connection.\n");
        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
		for (i = 0; i < strlen(buff); i++) {
        	Write(connfd, buff + i, 1);
		}

		Close(connfd);
	}
}
