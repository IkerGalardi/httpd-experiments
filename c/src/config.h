#ifndef _CONFIG_H
#define _CONFIG_H

#include <stdint.h>

typedef struct {
    uint16_t port;
} Config;

Config config_from_args(int argc, char** argv);

#endif // _CONFIG_H