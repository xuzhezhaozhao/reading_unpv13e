#include	"unp.h"
#include	<time.h>

int
main(int argc, char **argv)
{
	int					listenfd, connfd;
	socklen_t			len1, len2;
	struct sockaddr_in	servaddr, cliaddr;
	struct sockaddr_in *sa;
	struct sockaddr_storage ss;
	char				buff[MAXLINE];
	time_t				ticks;

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(13);	/* daytime server */

	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

	Listen(listenfd, LISTENQ);

		len2 = sizeof(ss);
		if (getsockname(listenfd, (SA *) &ss, &len2) < 0) {
			err_sys("getsockname error");
		}

		sa = (struct sockaddr_in *) &ss;
		printf("server ip: %s, port: %d\n", Inet_ntop(AF_INET, &sa->sin_addr, buff, sizeof(buff)), 
				ntohs(sa->sin_port));

	for ( ; ; ) {
		len1 = sizeof(cliaddr);
		connfd = Accept(listenfd, (SA *) &cliaddr, &len1);

		len2 = sizeof(ss);
		if (getsockname(connfd, (SA *) &ss, &len2) < 0) {
			err_sys("getsockname error");
		}

		sa = (struct sockaddr_in *) &ss;
		printf("server ip: %s, port: %d\n", Inet_ntop(AF_INET, &sa->sin_addr, buff, sizeof(buff)), 
				ntohs(sa->sin_port));

		printf("connection from %s, port %d\n",
			   Inet_ntop(AF_INET, &cliaddr.sin_addr, buff, sizeof(buff)),
			   ntohs(cliaddr.sin_port));

        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        Write(connfd, buff, strlen(buff));

		Close(connfd);
	}
}
