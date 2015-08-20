#include	"unp.h"

void
sig_chld(int signo)
{
	pid_t	pid;
	int		stat;

	(void)signo;

	pid = wait(&stat);
	printf("child %d terminated\n", pid);
	return;
}
