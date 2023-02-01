#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <threads.h>

#include "common.h"
#include "config.h"

typedef struct 
{
    int fd;
} ThreadParams;

static void* worker_thread(void* _params)
{
    ThreadParams params = *(ThreadParams*)_params;
    free(_params);

    printf("httpd(%ld): given %d socket\n", thrd_current(), params.fd);

    return NULL;
}

int main(int argc, char** argv)
{
    Config config = config_from_args(argc, argv);

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1)
    {
        fprintf(stderr, "httpd: could not create socket: %s\n", strerror(errno));
        goto close_socket;
    }

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(config.port),
        .sin_addr.s_addr = INADDR_ANY,
    };
    int errcode = bind(fd, (struct sockaddr*)&addr, sizeof(addr));
    if(errcode == -1)
    {
        fprintf(stderr, "httpd: could not bind the socket to port %d: %s\n", config.port, strerror(errno));
        goto close_socket;
    }

    errcode = listen(fd, 10);
    if(errcode == -1)
    {
        fprintf(stderr, "httpd: could not listen on port %d: %s\n", config.port, strerror(errno));
        goto close_socket;
    }

    printf("httpd: listening on localhost:%d\n", config.port);
    while(true)
    {
        socklen_t size = 0;
        int cfd = accept(fd, (struct sockaddr*)&addr, &size);
        if(cfd == -1)
        {
            switch(errno)
            {
                case EAGAIN:
                case EINTR:
                    continue;
            }

            fprintf(stderr, "httpd: error while accepting the connection: %s\n", strerror(errno));
        }

        thrd_t worker;
        ThreadParams* param = malloc(sizeof(ThreadParams));
        param->fd = cfd;
        if(thrd_create(&worker, worker_thread, param) != thrd_success)
        {
            fprintf(stderr, "httpd: could not create another thread for handling the request\n");
            close(cfd);
        }
    }

close_socket:
    close(fd);

    return 0;
}