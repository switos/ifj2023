#include "scanner.c"
token_t token;

int main() {
        str_init(&token.content);
        do
        {
            getToken(&token);
        } while (token.type != 0);
        
        str_free(&token.content);
}