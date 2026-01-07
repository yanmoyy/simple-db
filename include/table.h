#ifndef TABLE_H
#define TABLE_H

#include <stdint.h>

#define TABLE_MAX_PAGES 100

typedef struct table Table;

typedef struct table {
    uint32_t num_rows;
    void *pages[TABLE_MAX_PAGES];
} Table;

Table *new_table();
void free_table(Table *table);

#endif
