#include "repl.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_prompt() { printf("db > "); }

void repl(InputBuffer *input_buffer, Table *table)
{
    print_prompt();
    read_input(input_buffer);

    if (input_buffer->buffer[0] == '.') {
        switch (do_meta_command(input_buffer)) {
        case META_COMMAND_SUCCESS:
            return;
        case META_COMMAND_UNRECOGNIZED_COMMAND:
            printf("Unrecognized command\n");
            return;
        }
    }

    Statement statement;
    switch (prepare_statement(input_buffer, &statement)) {
    case PREPARE_SUCCESS:
        break;

    case PREPARE_SYNTAX_ERROR:
        printf("Syntax error. Could not parse statement.\n");
        return;

    case PREPARE_UNRECOGNIZED_STATEMENT:
        printf("Unrecognized keyword at start of '%s'.\n",
            input_buffer->buffer);
        return;
    }

    switch (execute_statement(&statement, table)) {
    case EXECUTE_SUCCESS:
        printf("Executed.\n");
        return;
    case EXECUTE_TABLE_FULL:
        printf("Error: Table is full.\n");
        return;
    case EXECUTE_UNRECOGNIZED_STATEMENT:
        printf("Unrecognized statement.\n");
        return;
    }
}

MetaCommandResult do_meta_command(InputBuffer *input_buffer)
{
    if (strcmp(input_buffer->buffer, ".exit") == 0) {
        exit(EXIT_SUCCESS);
    } else {
        return META_COMMAND_UNRECOGNIZED_COMMAND;
    }
}

PrepareResult prepare_statement(InputBuffer *input_buffer, Statement *statement)
{
    if (strncmp(input_buffer->buffer, "insert", 6) == 0) {
        statement->type = STATEMENT_INSERT;
        int args_assigned = sscanf(
            input_buffer->buffer, "insert %d %s %s",
            &(statement->row_to_insert.id),
            statement->row_to_insert.username,
            statement->row_to_insert.email);

        if (args_assigned != 3) {
            return PREPARE_SYNTAX_ERROR;
        }

        return PREPARE_SUCCESS;
    }
    if (strcmp(input_buffer->buffer, "select") == 0) {
        statement->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }

    return PREPARE_UNRECOGNIZED_STATEMENT;
}

ExecuteResult execute_statement(Statement *statement, Table *table)
{
    switch (statement->type) {
    case STATEMENT_INSERT:
        return execute_insert(statement, table);
    case STATEMENT_SELECT:
        return execute_select(statement, table);
    }
    return EXECUTE_UNRECOGNIZED_STATEMENT;
}

ExecuteResult execute_insert(Statement *statement, Table *table)
{
    if (table->num_rows >= TABLE_MAX_ROWS) {
        return EXECUTE_TABLE_FULL;
    }

    Row *row_to_insert = &(statement->row_to_insert);

    serialize_row(row_to_insert, row_slot(table, table->num_rows));
    table->num_rows++;

    return EXECUTE_SUCCESS;
}

ExecuteResult execute_select(Statement *statement, Table *table)
{
    (void)statement;

    Row row;
    for (uint32_t i = 0; i < table->num_rows; i++) {
        deserialize_row(row_slot(table, i), &row);
        print_row(&row);
    }
    return EXECUTE_SUCCESS;
}
