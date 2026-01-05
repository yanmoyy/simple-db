#include "db.h"
#include <stdbool.h>

int main()
{
    InputBuffer *input_buffer = new_input_buffer();
    while (true) {
        repl(input_buffer);
    }
}
