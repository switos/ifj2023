#include "str.c"

typedef enum {
    T_EOF,
    T_ID,
    T_LET,
    T_VAR,
    T_DOUBLE,
    T_INT,
    T_STRING,
    T_INT_LIT,
    T_FLOAT_LIT,
    T_STRING_LIT,
    T_ELSE,
    T_FUNC,
    T_IF,
    T_NIL,
    T_RETURN,
    T_WHILE,
    T_OP_BRACE,
    T_CL_BRACE,
    T_OP_PAR,
    T_CL_PAR,
    T_ASTER,
    T_SLASH,
    T_PLUS,
    T_MINUS,
    T_ARROW,
    T_ASSING,
    T_COLON,
    T_EQUAL,
    T_NOT,
    T_NOT_EQUAL,
    T_LESS,
    T_LESS_EQUAL,
    T_GREATER,
    T_GREATER_EQUAL,
    T_COND_TERN,
    T_NIL_COAL,
    T_COMMA,
    T_SEMICOLON,
} token_type;

/**
 * @brief token structure
 * 
 * @param string content
 * @param token_type type
 * 
 * @return T_EOF == 0 || 
 * T_ID == 1 || 
 * T_LET == 2 ||
 * T_VAR == 3 ||
 * T_DOUBLE == 4 ||
 * T_INT == 5 ||
 * T_STRING == 6 ||
 * T_INT_LIT == 7 ||
 * T_FLOAT_LIT == 8 ||
 * T_STRING_LIT == 9 ||
 * T_COLON == 10 ||
 * T_EQUAL == 11 ||
 * T_INT == 12 ||
 * T_INT == 13 ||
 * 
 */
typedef struct token
{
    string content;
    token_type type;
} token_t;
