#include "tcp_server.h"

#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

TcpServer* tcpserver_create(int port)
{
    TcpServer* result = malloc(sizeof(TcpServer));
    if(result == NULL)
        return NULL;

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1)
    {
        fprintf(stderr, "httpd: could not create socket: %s\n", strerror(errno));
        goto close_socket;
    }

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(port),
        .sin_addr.s_addr = INADDR_ANY,
    };
    int errcode = bind(fd, (struct sockaddr*)&addr, sizeof(addr));
    if(errcode == -1)
    {
        fprintf(stderr, "httpd: could not bind the socket to port %d: %s\n", port, strerror(errno));
        goto close_socket;
    }

    errcode = listen(fd, 10);
    if(errcode == -1)
    {
        fprintf(stderr, "httpd: could not listen on port %d: %s\n", port, strerror(errno));
        goto close_socket;
    }

    result->fd = fd;
    return result;

close_socket:
    close(fd);
    return NULL;

}

void tcpserver_listen(TcpServer* server, OnConnectionCallback callback)
{
    while(true)
    {
        socklen_t size = 0;
        struct sockaddr_in addr = {0};
        int cfd = accept(server->fd, (struct sockaddr*)&addr, &size);

        if(cfd == -1 && errno != EAGAIN && errno != EINTR)
            fprintf(stderr, "httpd: error while accepting the connection: %s\n", strerror(errno));


        thrd_t worker;
        int* param = malloc(sizeof(int));
        *param = cfd;
        if(thrd_create(&worker, callback, param) != thrd_success)
        {
            fprintf(stderr, "httpd: could not create another thread for handling the request\n");
            close(cfd);
        }
    }
}

void tcpserver_destroy(TcpServer* server)
{
    close(server->fd);
    free(server);
}