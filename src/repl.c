#include "db.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_prompt() { printf("db > "); }

void repl(InputBuffer *input_buffer)
{
    print_prompt();
    read_input(input_buffer);
    if (strcmp(input_buffer->buffer, ".exit") == 0) {
        close_input_buffer(input_buffer);
        exit(EXIT_SUCCESS);
    } else {
        printf("Unrecognized command '%s'.\n", input_buffer->buffer);
    }
}
