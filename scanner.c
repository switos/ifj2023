#include "token.h"

string tokenContent;
token_t token;

int identifierState(char symbol, token_t * token) {
    if ((symbol>64&&symbol<91) || (symbol>96 && symbol<123) || (symbol>47 && symbol<58) || (symbol == 95) ) {
        if (!str_add_char(&token->content, symbol)){
            fprintf(stderr,"Enternal error in identiferState in scanner\n");
            exit(99);
        }
        symbol = getc(stdin);
        identifierState(symbol, token);
    } else {
        printf("Succes, identifier is %s\n", token->content.str);
        token->type = 1;
        return 0; //succes, return the identifier, clean buffer 
    }
}

int underscoreState(char symbol, token_t * token) {
    if ((symbol>64&&symbol<91) || (symbol>96 && symbol<123) || (symbol>47 && symbol<58)) {
            if (!str_add_char(&token->content, symbol)){
                fprintf(stderr,"Enternal error in underscoreState in scanner\n");
                exit(99);
            }
            symbol = getc(stdin);
            identifierState(symbol, token);
    } else {
        fprintf(stderr,"Lexical error in underscore state\n");
        exit(1);
    }
}

int startState(char symbol, token_t * token) {
    if (symbol == 10 || symbol == 32) { //White symbols
        symbol = getc(stdin);
        startState(symbol, token);
    } else if(symbol == 95) {
            symbol = getc(stdin);
            underscoreState(symbol, token);
    } else if ((symbol>64&&symbol<91) || (symbol>96 && symbol<123)) {
            if (!str_add_char(&token->content, symbol))
                exit(1);
            symbol = getc(stdin);
            identifierState(symbol, token);
    } else if (symbol == EOF){
        token->type = 0;
        printf("Succes, EOF is found\n");
    }
    
}

int getToken(token_t * token) {
    str_clear(&token->content);
    char symbol = getc(stdin);
    startState(symbol, token);
}