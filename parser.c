#include "scanner.c"
token_t token;



int parse() {
    getToken(&token);
    if (token.type == T_LET) {
        getToken(&token);
    } else if(token.type == T_EOF) {
        return NO_ERR;
    } else {
        fprintf(stderr, "Syntaxe error in parse rule\n");
        return SYNTAX_ERR;
    }
}

int main() {
        str_init(&token.content);
        int result = parse();
        str_free(&token.content);
        if(result){
            exit(result);
        }
}