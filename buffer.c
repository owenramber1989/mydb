#include "buffer.h"

InputBuffer *new_input_buffer() {
    InputBuffer *newbuffer = (InputBuffer *)malloc(sizeof(InputBuffer));
    newbuffer->buffer = NULL;
    newbuffer->input_length = 0;
    newbuffer->buffer_length = 0;
    return newbuffer;
}
