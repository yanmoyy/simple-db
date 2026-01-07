#ifndef ROW_H
#define ROW_H

#include "table.h"
#include <stdint.h>

#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 255

typedef struct row {
    uint32_t id;
    char username[COLUMN_USERNAME_SIZE];
    char email[COLUMN_EMAIL_SIZE];
} Row;

#define size_of_attribute(Struct, Attribute) sizeof(((Struct *)0)->Attribute)

void serialize_row(Row *source, void *destination);
void deserialize_row(void *source, Row *destination);

void print_row(Row *row);
void *row_slot(Table *table, uint32_t row_num);

#endif
