#ifndef _TCP_SERVER_H
#define _TCP_SERVER_H

#include <stdbool.h>

typedef struct {
    int fd;
} TcpServer;

typedef int*(OnConnectionCallback)(void* fd);

TcpServer* tcpserver_create(int port);

void tcpserver_listen(TcpServer* server, OnConnectionCallback callback);

void tcpserver_destroy(TcpServer* server);

#endif // _TCP_SERVER_H