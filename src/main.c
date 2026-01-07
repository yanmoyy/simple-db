#include "repl.h"

#include <stdbool.h>

int main()
{
    Table *table = new_table();
    InputBuffer *input_buffer = new_input_buffer();
    while (true) {
        repl(input_buffer, table);
    }
}
