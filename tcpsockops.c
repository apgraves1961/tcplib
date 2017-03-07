/* tcpsockops.c - functions for sync. tcp read and write, with timeouts

Change log:
12/02/2008 APG First try
*/

#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include "tcpsockops.h"

static int timedout;

static void timeoutsub (int n)
{
	timedout = 1;
}

// write len bytes from buf to sock, waiting secs before timing out
// returns: 0 = OK, -1 = timed out, -2 = write failed immediately
int tcp_writetosocket (int sock, char *buf, int len, int secs) 
{
	int r;

	timedout = 0;
	if (secs > 0) {
		signal(SIGALRM, timeoutsub);
		alarm(secs);
	}

	r = write(sock, buf, len);

	if (secs > 0) {
		alarm(0);
		signal(SIGALRM, SIG_IGN);
	}

	if (timedout) {
		return -1;
	}

	if (r < len) {
		return -2;
	}

	return 0;
}

// read up to len bytes from sock to buf, waiting secs before timing out
// returns: >0 = # bytes read, 0 = EOF, -1 = timed out, -2 = read failed immediately
int tcp_readfromsocket (int sock, char *buf, int len, int secs)
{
	int l, r;

	timedout = 0;
	if (secs > 0) {
		signal(SIGALRM, timeoutsub);
		alarm(secs);
	}

	l = 0;

	while (l < len) {
		if (l > 0) {
			usleep(1000);
		}
		r = read (sock, buf, len - l);

		if (r < 1 || timedout) {
			break;
		}

		l += r;
	}

	if (secs > 0) {
		alarm(0);
		signal(SIGALRM, SIG_IGN);
	}

	if (timedout) {
		return -1;
	}

	if (r == -1) {
		return -2;
	}

	return l;
}
