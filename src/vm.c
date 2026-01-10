#include "cursor.h"
#include "repl.h"
#include "row.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern const uint32_t TABLE_MAX_ROWS;

void print_prompt() { printf("db > "); }

void repl(InputBuffer *input_buffer, Table *table)
{
    print_prompt();
    read_input(input_buffer);

    if (input_buffer->buffer[0] == '.') {
        switch (do_meta_command(input_buffer, table)) {
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
    case PREPARE_NEGATIVE_ID:
        printf("ID must be positive.\n");
        return;

    case PREPARE_STRING_TOO_LONG:
        printf("String is too long.\n");
        return;

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
        printf("Error: Table full.\n");
        return;
    case EXECUTE_UNRECOGNIZED_STATEMENT:
        printf("Unrecognized statement.\n");
        return;
    }
}

MetaCommandResult do_meta_command(InputBuffer *input_buffer, Table *table)
{
    if (strcmp(input_buffer->buffer, ".exit") == 0) {
        db_close(table);
        exit(EXIT_SUCCESS);
    } else {
        return META_COMMAND_UNRECOGNIZED_COMMAND;
    }
}

PrepareResult prepare_insert(InputBuffer *input_buffer, Statement *statement)
{
    statement->type = STATEMENT_INSERT;

    char *keyword = strtok(input_buffer->buffer, " ");
    if (strcmp(keyword, "insert") != 0) {
        return PREPARE_UNRECOGNIZED_STATEMENT;
    }

    char *id_string = strtok(NULL, " ");
    char *username = strtok(NULL, " ");
    char *email = strtok(NULL, " ");

    if (id_string == NULL || username == NULL || email == NULL) {
        return PREPARE_SYNTAX_ERROR;
    }

    int id = atoi(id_string);
    if (id < 0) {
        return PREPARE_NEGATIVE_ID;
    }
    if (strlen(username) > COLUMN_EMAIL_SIZE) {
        return PREPARE_STRING_TOO_LONG;
    }
    if (strlen(email) > COLUMN_EMAIL_SIZE) {
        return PREPARE_STRING_TOO_LONG;
    }

    statement->row_to_insert.id = id;
    strcpy(statement->row_to_insert.username, username);
    strcpy(statement->row_to_insert.email, email);

    return PREPARE_SUCCESS;
}

PrepareResult prepare_statement(InputBuffer *input_buffer, Statement *statement)
{
    if (strncmp(input_buffer->buffer, "insert", 6) == 0) {
        return prepare_insert(input_buffer, statement);
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
    Cursor *cursor = table_end(table);

    serialize_row(row_to_insert, cursor_value(cursor));
    table->num_rows++;

    return EXECUTE_SUCCESS;
}

ExecuteResult execute_select(Statement *statement, Table *table)
{
    (void)statement;

    Cursor *cursor = table_start(table);

    Row row;
    while (!(cursor->end_of_table)) {
        deserialize_row(cursor_value(cursor), &row);
        print_row(&row);
        cursor_advance(cursor);
    }

    free(cursor);

    return EXECUTE_SUCCESS;
}
