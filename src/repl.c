#include "db.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_prompt() { printf("db > "); }

void repl(InputBuffer *input_buffer)
{
    print_prompt();
    read_input(input_buffer);

    if (strcmp(input_buffer->buffer, ".exit") == 0) {
        if (input_buffer->buffer[0] == '.') {
            switch (do_meta_command(input_buffer)) {
            case META_COMMAND_SUCCESS:
                return;
            case META_COMMAND_UNRECOGNIZED_COMMAND:
                printf("Unrecognized command\n");
                return;
            }
        }
    }

    Statement statement;
    switch (prepare_statement(input_buffer, &statement)) {
    case PREPARE_SUCCESS:
        break;
    case PREPARE_UNRECOGNIZED_STATEMENT:
        printf("Unrecognized keyword at start of '%s'.\n",
            input_buffer->buffer);
        return;
    }

    execute_statement(&statement);
    printf("Executed.\n");
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
        return PREPARE_SUCCESS;
    }
    if (strcmp(input_buffer->buffer, "select") == 0) {
        statement->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }

    return PREPARE_UNRECOGNIZED_STATEMENT;
}

void execute_statement(Statement *statement)
{
    switch (statement->type) {
    case STATEMENT_INSERT:
        printf("This is where we would do an insert.\n");
        break;
    case STATEMENT_SELECT:
        printf("This is where we would do a select.\n");
        break;
    }
}
