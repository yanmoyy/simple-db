#ifndef TREE_H
#define TREE_H

#include "cursor.h"
#include "print.h"
#include "row.h"
#include <stdint.h>
#include <stdio.h>
typedef enum {
    NODE_INTERNAL,
    NODE_LEAF,
} NodeType;

/* getters (leaf)*/
uint32_t *leaf_node_num_cells(void *node);
void *leaf_node_cell(void *node, uint32_t cell_num);
uint32_t *leaf_node_key(void *node, uint32_t cell_num);
void *leaf_node_value(void *node, uint32_t cell_num);

/* getters (internal)*/
uint32_t *internal_node_num_keys(void *node);
uint32_t *internal_node_right_child(void *node);
uint32_t *internal_node_cell(void *node, uint32_t cell_num);
uint32_t *internal_node_child(void *node, uint32_t child_num);
uint32_t *internal_node_key(void *node, uint32_t key_num);

uint32_t get_node_max_key(void *node);

/* logic */
void initialize_leaf_node(void *node);
void initialize_internal_node(void *node);

void leaf_node_insert(Cursor *cursor, uint32_t key, Row *value);
Cursor *leaf_node_find(Table *table, uint32_t page_num, uint32_t key);
void leaf_node_split_and_insert(Cursor *cursor, uint32_t key, Row *value);
void create_new_root(Table *table, uint32_t right_child_page_num);

NodeType get_node_type(void *node);
void set_node_type(void *node, NodeType type);

bool is_node_root(void *node);
void set_node_root(void *node, bool is_root);

void print_constants();
void print_tree(Pager *pager, uint32_t page_num, uint32_t indentation_level);
#endif
