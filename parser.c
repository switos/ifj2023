#include "parser.h"
token_t token;

int unmutVarTypeInit() {
    return 1;
}
int bottomtotop() {
    return 1;
}

int unmutableVariable() {
    if (token.type ==  T_ID) {
        getToken(&token);
        if (token.type == T_COLON) {
            unmutVarTypeInit();
        } else if (T_FLOAT || T_STRING || T_INT || T_ID) {
            bottomtotop();
        }
    }
    
    return SYNTAX_ERR;
}

int parse() {

    if (token.type == T_LET) {
        getToken(&token);
        return unmutableVariable();
    } else if(token.type == T_EOF) {
        return NO_ERR;
    } else {
        return printErrorAndReturn("Syntaxe error in parse rule", SYNTAX_ERR);
        // fprintf(stderr, "Syntaxe error in parse rule\n");
        // return SYNTAX_ERR;
    }
}

int main() {
        str_init(&token.content);
        int result;
        do {
            result = getToken(&token);
            if(result)
                break;
            result = parse();
            if(result)
                break;    
        } while (token.type != T_EOF);
        str_free(&token.content);
        if (result){
            exit(result);
        }
}