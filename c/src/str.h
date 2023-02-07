#ifndef _STR_H
#define _STR_H

#include <stddef.h>

typedef struct {
    char* buffer;
    size_t size;
} String;

String string_create(size_t size);

void string_destroy(String* str);

void string_append(String* str, char* buffer, size_t size);

#endif // _STR_H