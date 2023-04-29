#include "buffer.h"
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 255
#define sizeof_of_attribute(Struct, Attribute) sizeof(((Struct *)0)->Attribute)
const uint32_t PAGE_SIZE = 4096;
#define TABLE_MAX_PAGE 100

#ifndef MYDB_HEADER
#define MYDB_HEADER

typedef enum {
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

typedef enum {
    PREPARE_SUCCESS,
    PREPARE_SYNTAX_ERROR,
    PREPARE_STRING_TOO_LONG,
    PREPARE_NEGATIVE_ID,
    PREPARE_UNRECOGNIZED_STATEMENT
} PrepareResult;
typedef enum { EXECUTE_SUCCESS, EXECUTE_TABLE_FULL } ExecuteResult;
typedef enum { STATEMENT_INSERT, STATEMENT_SELECT } StatementType;

typedef struct {
    uint32_t id;
    char username[COLUMN_USERNAME_SIZE + 1];
    char email[COLUMN_EMAIL_SIZE + 1];
} Row;

typedef struct {
    StatementType type;
    Row row_to_insert;
} Statement;

typedef struct {
    int file_descriptor;
    uint32_t file_length;
    void *pages[TABLE_MAX_PAGE];
} Pager;
typedef struct {
    Pager *pager;
    uint32_t num_rows;
} Table;

typedef struct {
    Table *table;
    uint32_t row_num;
    bool end_of_table;
} Cursor;

typedef struct {
    InputBuffer *input_buffer;
    Table *table;
    Cursor *cursor;
} DB;
#endif

const uint32_t ID_SIZE = sizeof_of_attribute(Row, id);
const uint32_t USERNAME_SIZE = sizeof_of_attribute(Row, username);
const uint32_t EMAIL_SIZE = sizeof_of_attribute(Row, email);
const uint32_t ID_OFFSET = 0;
const uint32_t USERNAME_OFFSET = ID_OFFSET + ID_SIZE;
const uint32_t EMAIL_OFFSET = USERNAME_OFFSET + USERNAME_SIZE;
const uint32_t ROW_SIZE = ID_SIZE + USERNAME_SIZE + EMAIL_SIZE;

const uint32_t ROWS_PER_PAGE = PAGE_SIZE / ROW_SIZE;
const uint32_t TABLE_MAX_ROWS = ROWS_PER_PAGE * TABLE_MAX_PAGE;
void read_input(InputBuffer *);

void print_prompt();
void close_input_buffer(InputBuffer *);

MetaCommandResult exe_meta_command(InputBuffer *, Table *);
PrepareResult prepare_statement(InputBuffer *, Statement *);
ExecuteResult execute_statement(Statement *, Table *);
void serialize_row(Row *, void *);
void deserialize_row(void *, Row *);
void *access_row(Table *, uint32_t);
void *get_page(Pager *, uint32_t);
ExecuteResult execute_insert(Statement *, Table *);
ExecuteResult execute_select(Statement *, Table *);
Table *access_db(const char *filename);
void free_table(Table *);
PrepareResult prepare_insert(InputBuffer *, Statement *);
Pager *access_pager(const char *filename);
void close_db(Table *table);
void pager_flush(Pager *, uint32_t, uint32_t);
