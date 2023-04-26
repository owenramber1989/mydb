#include "mydb.h"
#include "buffer.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

int main(int argc, char *argv[]) {
    InputBuffer *input_buffer = new_input_buffer();
    while (true) {
        print_prompt();
        read_input(input_buffer);
        if (input_buffer->buffer[0] == '.') {
            switch (exe_meta_command(input_buffer)) {
            case (META_COMMAND_SUCCESS):
                continue;
            case (META_COMMAND_UNRECOGNIZED_COMMAND):
                printf("Unrecognized command '%s'.\n", input_buffer->buffer);
                continue;
            }
        }
        Statement statement;
        switch (prepare_statement(input_buffer, &statement)) {
        case (PREPARE_SUCCESS):
            break;
        case (PREPARE_SYNTAX_ERROR):
            printf("Syntax error. Could not parse statement.\n");
            continue;
        }
        execute_statement(&statement);
        printf("Executed.\n");
    }
}

void read_input(InputBuffer *input_buffer) {
    ssize_t bytes_read =
        getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);
    if (bytes_read <= 0) {
        printf("Error reading input\n");
        exit(EXIT_FAILURE);
    }
    input_buffer->input_length = bytes_read - 1;
    input_buffer->buffer[bytes_read - 1] = 0;
}

void print_prompt() { printf("mydb > "); }
void close_input_buffer(InputBuffer *input_buffer) {
    free(input_buffer->buffer);
    free(input_buffer);
}
MetaCommandResult exe_meta_command(InputBuffer *input_buffer) {
    if (strcmp(input_buffer->buffer, ".exit") == 0) {
        printf("good bye\n");
        exit(EXIT_SUCCESS);
    } else {
        return META_COMMAND_UNRECOGNIZED_COMMAND;
    }
}

PrepareResult prepare_statement(InputBuffer *input_buffer,
                                Statement *statement) {
    if (strncmp(input_buffer->buffer, "insert", 6) == 0) {
        statement->type = STATEMENT_INSERT;
        return PREPARE_SUCCESS;
    }
    if (strcmp(input_buffer->buffer, "select") == 0) {
        statement->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }

    return PREPARE_SYNTAX_ERROR;
}

void execute_statement(Statement *statement) {
    switch (statement->type) {
    case (STATEMENT_INSERT):
        printf("This is where we would do an insert.\n");
        break;
    case (STATEMENT_SELECT):
        printf("This is where we would do a select.\n");
        break;
    }
}
