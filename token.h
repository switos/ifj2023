#include "str.c"

typedef enum {
    T_EOF,
    T_ID,
    T_LET,
    T_INT,
    T_FLOAT,
    T_INT_EXP,
    T_FLOAT_EXP,
    T_STRING,
} token_type;

typedef struct token
{
    string content;
    token_type type;
} token_t;
