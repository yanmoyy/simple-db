#ifndef TREE_H
#define TREE_H

#include "cursor.h"
#include "row.h"
#include <stdint.h>
typedef enum {
    NODE_INTERNAL,
    NODE_LEAF,
} NodeType;

uint32_t *leaf_node_num_cells(void *node);
void *leaf_node_cell(void *node, uint32_t cell_num);
uint32_t *leaf_node_key(void *node, uint32_t cell_num);
void *leaf_node_value(void *node, uint32_t cell_num);
void initialize_leaf_node(void *node);
void leaf_node_insert(Cursor *cursor, uint32_t key, Row *value);
void print_constants();
void print_leaf_node(void *node);
#endif
