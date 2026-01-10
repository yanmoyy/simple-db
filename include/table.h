#ifndef TABLE_H
#define TABLE_H

#include "pager.h"
#include <stdint.h>

typedef struct table {
    Pager *pager;
    uint32_t root_page_num;
} Table;

Table *db_open(const char *filename);
void db_close(Table *table);

#endif
