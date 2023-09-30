#include "scanner.c"
token_t token;



int parse() {
    getToken(&token);
    while (token.type != T_EOF) {
        getToken(&token);
    }
    return 0;
    //if (token.type == T_LET) {
    //    getToken(&token);
    //} else if(token.type == T_EOF) {
    //    return NO_ERR;
    //} else {
    //    return printErrorAndReturn("Syntaxe error in parse rule", SYNTAX_ERR);
    //    // fprintf(stderr, "Syntaxe error in parse rule\n");
    //    // return SYNTAX_ERR;
    //}
}

int main() {
        str_init(&token.content);
        int result = parse();
        str_free(&token.content);
        if (result){
            exit(result);
        }
}