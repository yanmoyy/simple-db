#ifndef TREE_H
#define TREE_H

#include "cursor.h"
#include "row.h"
#include <stdint.h>
typedef enum {
    NODE_INTERNAL,
    NODE_LEAF,
} NodeType;

/* getters (common)*/
uint32_t *node_parent(void *node);

/* getters (leaf)*/
uint32_t *leaf_node_num_cells(void *node);
void *leaf_node_cell(void *node, uint32_t cell_num);
uint32_t *leaf_node_key(void *node, uint32_t cell_num);
void *leaf_node_value(void *node, uint32_t cell_num);
uint32_t *leaf_node_next_leaf(void *node);

/* getters (internal)*/
uint32_t *internal_node_num_keys(void *node);
uint32_t *internal_node_right_child(void *node);
uint32_t *internal_node_cell(void *node, uint32_t cell_num);
uint32_t *internal_node_child(void *node, uint32_t child_num);
uint32_t *internal_node_key(void *node, uint32_t key_num);

uint32_t get_node_max_key(void *node);

/* logic */
void create_new_root(Table *table, uint32_t right_child_page_num);

NodeType get_node_type(void *node);
void set_node_type(void *node, NodeType type);

bool is_node_root(void *node);
void set_node_root(void *node, bool is_root);

void leaf_node_insert(Cursor *cursor, uint32_t key, Row *value);
Cursor *leaf_node_find(Table *table, uint32_t page_num, uint32_t key);
void leaf_node_split_and_insert(Cursor *cursor, uint32_t key, Row *value);

void initialize_leaf_node(void *node);
void initialize_internal_node(void *node);

void internal_node_insert(Table *table, uint32_t parent_page_num, uint32_t child_page_num);
uint32_t internal_node_find_child(void *node, uint32_t key);
Cursor *internal_node_find(Table *table, uint32_t page_num, uint32_t key);
void update_internal_node_key(void *node, uint32_t old_key, uint32_t new_key);

/* print */
void print_constants();
void print_tree(Pager *pager, uint32_t page_num, uint32_t indentation_level);
#endif
