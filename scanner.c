#include "scanner.h"

string tokenContent;
token_t token;

int identifierState(char symbol, token_t * token) {
    if (isalnum(symbol) || symbol == '_') {
        if (!str_add_char(&token->content, symbol)) {
            fprintf(stderr, "Enternal error in identiferState in scanner\n");
            exit(ERROR_INTERNAL);
        }
        symbol = getc(stdin);
        identifierState(symbol, token);
    } else if (isspace(symbol)) {
        printf("Success, identifier is %s\n", token->content.str);
        token->type = T_ID;
        return NO_ERR; // success, return the identifier, clean buffer 
    } else {
        return LEX_ERR;
    }
}

int underscoreState(char symbol, token_t * token) {
    if (isalnum(symbol)) {
        if (!str_add_char(&token->content, symbol)){
            fprintf(stderr, "Enternal error in underscoreState in scanner\n");
            exit(ERROR_INTERNAL);
        }
        symbol = getc(stdin);
        identifierState(symbol, token);
    } else {
        fprintf(stderr, "Lexical error in underscore state\n");
        exit(LEX_ERR);
    }
}

int intState(char symbol, token_t * token) {
    if (isnumber(symbol)) {
        if (!str_add_char(&token->content, symbol))
            exit(1);
        symbol = getc(stdin);
        intState(symbol, token);
    } else if (symbol == '.') {
        if (!str_add_char(&token->content, symbol))
            exit(LEX_ERR);
        symbol = getc(stdin);
        if (isnumber(symbol)) {
            floatState(symbol, token);
        } else {
            exit(LEX_ERR);
        }
    } else if (symbol == 'E' || symbol == 'e') {
        if (!str_add_char(&token->content, symbol))
            exit(LEX_ERR);
        symbol = getc(stdin);
        if (symbol == '+' || symbol == '-' || isnumber(symbol)) {
            if (!str_add_char(&token->content, symbol))
                exit(1);
            symbol = getc(stdin);
            intExpState(symbol, token);
        } else {
            exit(LEX_ERR);
        }
    } else if (isspace(symbol)) {
        printf("Success, identifier is %s\n", token->content.str);
        token->type = T_INT;
        return NO_ERR; // success, return the identifier, clean buffer 
    } else {
        exit(LEX_ERR);
    }
}

int floatState(char symbol, token_t * token) {
    if (isnumber(symbol)) {
        if (!str_add_char(&token->content, symbol))
            exit(1);
        symbol = getc(stdin);
        floatState(symbol, token);
    } else if (symbol == 'E' || symbol == 'e') {
        if (!str_add_char(&token->content, symbol))
            exit(LEX_ERR);
        symbol = getc(stdin);
        if (symbol == '+' || symbol == '-' || isnumber(symbol)) {
            if (!str_add_char(&token->content, symbol))
                exit(1);
            symbol = getc(stdin);
            floatExpState(symbol, token);
        } else {
            exit(LEX_ERR);
        }
    } else if (isspace(symbol)) {
        printf("Success, identifier is %s\n", token->content.str);
        token->type = T_FLOAT;
        return NO_ERR; // success, return the identifier, clean buffer 
    } else {
        exit(LEX_ERR);
    }
}

int intExpState(char symbol, token_t * token) {
    if (isnumber(symbol)) {
        if (!str_add_char(&token->content, symbol))
            exit(1);
        symbol = getc(stdin);
        floatState(symbol, token);
    } else if (isspace(symbol)) {
        printf("Success, identifier is %s\n", token->content.str);
        token->type = T_INT_EXP;
        return NO_ERR; // success, return the identifier, clean buffer 
    } else {
        exit(LEX_ERR);
    }
}

int floatExpState(char symbol, token_t * token) {
    if (isnumber(symbol)) {
        if (!str_add_char(&token->content, symbol))
            exit(1);
        symbol = getc(stdin);
        floatState(symbol, token);
    } else if (isspace(symbol)) {
        printf("Success, identifier is %s\n", token->content.str);
        token->type = T_FLOAT_EXP;
        return NO_ERR; // success, return the identifier, clean buffer 
    } else {
        exit(LEX_ERR);
    }
}

int startState(char symbol, token_t * token) {
    if (isspace(symbol)) { // White symbols
        symbol = getc(stdin);
        startState(symbol, token);
    } else if (symbol == '_') { // underscore
        symbol = getc(stdin);
        underscoreState(symbol, token);
    } else if (isalpha(symbol)) {
        if (!str_add_char(&token->content, symbol))
            exit(LEX_ERR);
        symbol = getc(stdin);
        identifierState(symbol, token);
    } else if (isnumber(symbol)) {
        if (!str_add_char(&token->content, symbol))
            exit(LEX_ERR);
        symbol = getc(stdin);
        intState(symbol, token);
    } else if (symbol == EOF) {
        token->type = 0;
        printf("Success, EOF is found\n");
    }
    
}

int getToken(token_t * token) {
    str_clear(&token->content);
    char symbol = getc(stdin);
    startState(symbol, token);
}