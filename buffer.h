#include <stdio.h>
#include <stdlib.h>

#ifndef BUFFER_HEADER
#define BUFFER_HEADER

typedef struct {
    char *buffer;
    size_t buffer_length;
    ssize_t input_length;
} InputBuffer;

InputBuffer *new_input_buffer();

#endif
