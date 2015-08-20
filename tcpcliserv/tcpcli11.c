/* Use standard echo server; baseline measurements for nonblocking version */
#include	"unp.h"

void sig_pipe(int signo) {
	(void)signo;

	printf("handling sigpipe, broken pipe\n");
	exit(0);
}

int
main(int argc, char **argv)
{
	int					sockfd;
	struct sockaddr_in	servaddr;

	if (argc != 2)
		err_quit("usage: tcpcli <IPaddress>");

	sockfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

	Connect(sockfd, (SA *) &servaddr, sizeof(servaddr));

	Signal(SIGPIPE, sig_pipe);

	str_cli(stdin, sockfd);		/* do it all */

	exit(0);
}
