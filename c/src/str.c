#include "str.h"

#include <stdlib.h>
#include <string.h>

String string_create(size_t size)
{
    String result = {0};

    if(size != 0)
    {
        result.buffer = malloc(size);
        memset(result.buffer, 0, size);
        result.size = size;
    }

    return result;
}

void string_destroy(String* str)
{
    free(str->buffer);
    str->size = 0;
}

void string_append(String* str, char* buffer, size_t size)
{
    size_t new_size = str->size + size;
    str->buffer = realloc(str->buffer, new_size);
    strcat(str->buffer, buffer);
}