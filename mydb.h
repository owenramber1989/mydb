#include "buffer.h"
#include <stdio.h>

typedef enum {
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

typedef enum { PREPARE_SUCCESS, PREPARE_SYNTAX_ERROR } PrepareResult;

typedef enum { STATEMENT_INSERT, STATEMENT_SELECT } StatementType;

typedef struct {
    StatementType type;
} Statement;

void read_input(InputBuffer *);

void print_prompt();
void close_input_buffer(InputBuffer *);

MetaCommandResult exe_meta_command(InputBuffer *);
PrepareResult prepare_statement(InputBuffer *, Statement *);
void execute_statement(Statement *);
