#include "parser.h"
token_t token;

int main() {
    str_init(&token.content);
    // int result = getToken(&token);
    int result = getToken(&token);
    while (token.type != T_EOF && !result) {
        result = getToken(&token);
        // printf("end\n");
        // result = getToken(&token);
    }
    str_free(&token.content);
}