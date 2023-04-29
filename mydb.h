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
typedef enum {
    EXECUTE_SUCCESS,
    EXECUTE_DUPLICATE_KEY,
    EXECUTE_TABLE_FULL
} ExecuteResult;
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
    uint32_t num_pages;
    void *pages[TABLE_MAX_PAGE];
} Pager;
typedef struct {
    Pager *pager;
    uint32_t root_page_num;
} Table;

typedef struct {
    Table *table;
    uint32_t page_num;
    uint32_t cell_num;
    bool end_of_table;
} Cursor;

typedef struct {
    InputBuffer *input_buffer;
    Table *table;
    Cursor *cursor;
} DB;

typedef enum { NODE_INTERNAL, NODE_LEAF } NodeType;

const uint32_t ID_SIZE = sizeof_of_attribute(Row, id);
const uint32_t USERNAME_SIZE = sizeof_of_attribute(Row, username);
const uint32_t EMAIL_SIZE = sizeof_of_attribute(Row, email);
const uint32_t ID_OFFSET = 0;
const uint32_t USERNAME_OFFSET = ID_OFFSET + ID_SIZE;
const uint32_t EMAIL_OFFSET = USERNAME_OFFSET + USERNAME_SIZE;
const uint32_t ROW_SIZE = ID_SIZE + USERNAME_SIZE + EMAIL_SIZE;

const uint32_t NODE_TYPE_SIZE = sizeof(uint8_t);
const uint32_t NODE_TYPE_OFFSET = 0;
const uint32_t IS_ROOT_SIZE = sizeof(uint8_t);
const uint32_t IS_ROOT_OFFSET = NODE_TYPE_SIZE;
const uint32_t PARENT_POINTER_SIZE = sizeof(uint32_t);
const uint32_t PARENT_POINTER_OFFSET = IS_ROOT_OFFSET + IS_ROOT_SIZE;
const uint32_t COMMON_NODE_HEADER_SIZE =
    NODE_TYPE_SIZE + IS_ROOT_SIZE + PARENT_POINTER_SIZE;

const uint32_t LEAF_NODE_NUM_CELLS_SIZE = sizeof(uint32_t);
const uint32_t LEAF_NODE_NUM_CELLS_OFFSET = COMMON_NODE_HEADER_SIZE;
const uint32_t LEAF_NODE_HEADER_SIZE =
    COMMON_NODE_HEADER_SIZE + LEAF_NODE_NUM_CELLS_SIZE;

/*
 * Leaf Node Body Layout
 */
const uint32_t LEAF_NODE_KEY_SIZE = sizeof(uint32_t);
const uint32_t LEAF_NODE_KEY_OFFSET = 0;
const uint32_t LEAF_NODE_VALUE_SIZE = ROW_SIZE;
const uint32_t LEAF_NODE_VALUE_OFFSET =
    LEAF_NODE_KEY_OFFSET + LEAF_NODE_KEY_SIZE;
const uint32_t LEAF_NODE_CELL_SIZE = LEAF_NODE_KEY_SIZE + LEAF_NODE_VALUE_SIZE;
const uint32_t LEAF_NODE_SPACE_FOR_CELLS = PAGE_SIZE - LEAF_NODE_HEADER_SIZE;
const uint32_t LEAF_NODE_MAX_CELLS =
    LEAF_NODE_SPACE_FOR_CELLS / LEAF_NODE_CELL_SIZE;

void read_input(InputBuffer *);

void print_prompt();
void print_constants();
void print_leaf_node(void *);
void close_input_buffer(InputBuffer *);

MetaCommandResult exe_meta_command(InputBuffer *, Table *);
PrepareResult prepare_statement(InputBuffer *, Statement *);
ExecuteResult execute_statement(Statement *, Table *);
void serialize_row(Row *, void *);
void deserialize_row(void *, Row *);
void *access_row(Cursor *);
void *get_page(Pager *, uint32_t);
ExecuteResult execute_insert(Statement *, Table *);
ExecuteResult execute_select(Statement *, Table *);
Table *access_db(const char *filename);
PrepareResult prepare_insert(InputBuffer *, Statement *);
Pager *access_pager(const char *filename);
void close_db(Table *table);
void pager_flush(Pager *, uint32_t);
void cursor_advance(Cursor *);
Cursor *table_head(Table *);
Cursor *table_find(Table *, uint32_t);
void *cursor_value(Cursor *);
Cursor *leaf_node_find(Table *, uint32_t, uint32_t);

uint32_t *leaf_node_num_cells(void *);
void *leaf_node_ceil(void *, uint32_t);
void *leaf_node_value(void *, uint32_t);
uint32_t *leaf_node_key(void *, uint32_t);
void initialize_leaf_node(void *);
void leaf_node_insert(Cursor *, uint32_t, Row *);

NodeType get_node_type(void *);
void set_node_type(void *, NodeType);
#endif
