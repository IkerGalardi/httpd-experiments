#include <stdio.h>

#include "common.h"
#include "config.h"

int main(int argc, char** argv)
{
    Config config = config_from_args(argc, argv);

    printf("Hello world, port %d\n", config.port);
    return 0;
}