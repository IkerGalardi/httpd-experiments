#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include <threads.h>

#include "common.h"
#include "config.h"
#include "tcp_server.h"

static int* worker_thread(void* _params)
{
    int fd = *(int*)_params;
    free(_params);

    printf("httpd(%ld): given %d socket\n", thrd_current(), fd);

    return NULL;
}

int main(int argc, char** argv)
{
    Config config = config_from_args(argc, argv);

    TcpServer* server = tcpserver_create(config.port);
    if(server == NULL)
        return 1;

    printf("httpd: listening on localhost:%d\n", config.port);
    tcpserver_listen(server, worker_thread);
}