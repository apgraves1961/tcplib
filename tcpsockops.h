#ifndef TCP_SOCKOPS_H
#define TCP_SOCKOPS_H 1

int tcp_writetosocket (int, char *, int, int);
int tcp_readfromsocket (int, char *, int, int);

#endif // TCP_SOCKOPS_H
