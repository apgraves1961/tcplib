#ifndef TCPSOCKOPS_H
#define TCPSOCKOPS_H 1

int tcp_writetosocket (int, char *, int, int);
int tcp_readfromsocket (int, char *, int, int);

#endif // TCPSOCKOPS_H
