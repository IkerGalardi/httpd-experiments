#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "common.h"
#include "config.h"

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

close_socket:
    close(fd);

    return 0;
}