#include "str.c"

typedef enum {
    T_PLUS,
    T_MINUS,
    T_MUL,
    T_DIV,
    T_ID,
    T_INT_LIT,
    T_FLOAT_LIT,
    T_STRING_LIT,
    T_EOF,
    T_LET,
    T_VAR,
    T_DOUBLE,
    T_INT,
    T_STRING,
    T_COLON,
    T_EQUAL,
} token_type;

typedef struct token
{
    string content;
    token_type type;
} token_t;
