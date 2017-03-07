/* tcpclientconnect.c - functions to open and close a TCP client connection

Change log:
12/01/2008 APG First try
*/
#include <string.h>
#include <sys/socket.h>
#include <sys/socketvar.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include "tcpclientconnect.h"

#define DEFAULT_WINSIZE 1024

/* attempt to form a TCP connection to server at host listening on port.
returns: -3 = unknown host
         -2 = unable to create socket
         -1 = stream connect attempt failed
        >=0 = file handle of connected socket
*/
int tcp_clientconnect (char *host, int port, int winsiz)
{
	struct sockaddr_in sa;
	struct hostent *hp = gethostbyname(host);
	int sk;
	int window_size;

	// unknown host
	if (!hp) return -3;

	// set up the sockaddr
	memset (&sa, 0, sizeof(struct sockaddr_in));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(port);
	memcpy (&(sa.sin_addr), hp->h_addr, hp->h_length);

	// create the socket
	if ((sk = socket(AF_INET,SOCK_STREAM,0)) == -1) {
		return -2;
	}

	// set the TCP window size
	window_size = (winsiz ? winsiz : DEFAULT_WINSIZE);
	setsockopt(sk, SOL_SOCKET, SO_SNDBUF, &window_size, sizeof(int));
	setsockopt(sk, SOL_SOCKET, SO_RCVBUF, &window_size, sizeof(int));

	// attempt the connection
	if (connect(sk, (struct sockaddr *)&sa, sizeof(struct sockaddr_in)) == -1) {
		close (sk);
		return -1;
	} else {
		return sk;
	}
}

void tcp_clientdisconnect (int sk)
{
	shutdown (sk, SHUT_RDWR);
	close (sk);
}
