#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include <threads.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <string.h>

#include "common.h"
#include "config.h"
#include "tcp_server.h"
#include "str.h"

#define TEMP_BUFFER_SIZE 255

static void wait_socket_message(int fd)
{
    fd_set fdset;
    FD_ZERO(&fdset);
    FD_SET(fd, &fdset);

    struct timeval wait_time = {
        .tv_sec = 5,
        .tv_usec = 0
    };

    int ret = select(1, &fdset, NULL, NULL, &wait_time);
    if(ret)
        return;
}

static int* worker_thread(void* _params)
{
    int fd = *(int*)_params;
    free(_params);

    char* tmp_buff = malloc(TEMP_BUFFER_SIZE);
    String request = string_create(0);

    wait_socket_message(fd);
    while(true)
    {
        int count = recv(fd, tmp_buff, TEMP_BUFFER_SIZE - 1, MSG_DONTWAIT);
        if(count == -1 && EAGAIN)
        {
            break;
        }

        tmp_buff[count] = 0;
        string_append(&request, tmp_buff, strlen(tmp_buff));
    }

    printf("httpd(%ld): received the next http request:\n%s\n", thrd_current(), request.buffer);

    string_destroy(&request);
    free(tmp_buff);
    close(fd);

    thrd_exit(0);

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