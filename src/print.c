#include "print.h"
#include <stdio.h>

void indent(uint32_t level)
{
    for (uint32_t i = 0; i < level; i++) {
        printf("  ");
    }
}
