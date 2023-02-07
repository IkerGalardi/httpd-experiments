#include "config.h"

#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>

Config config_from_args(int argc, char** argv)
{
    Config result = 
    {
        .port = 80, // Default HTTP protocol port
    };

    int c = 0;
    while ((c = getopt (argc, argv, "p:")) != -1)
    {
        switch(c)
        {
            case 'p':
                result.port = atoi(optarg);
                break;
            default:
                fprintf(stderr, "config: unknown argv flag %d passed\n", c);
        }
    }

    return result;
}