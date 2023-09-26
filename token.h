#include "str.c"

typedef enum {
    T_EOF,
    T_ID,
} token_type;

typedef struct token
{
    string content;
    token_type type;
}token_t;
