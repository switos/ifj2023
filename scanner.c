#include "scanner.h"

string tokenContent;
token_t token;
char symbol;

int identifierState(token_t * token) {
    if (isalnum(symbol) || symbol == '_') {
        if (!str_add_char(&token->content, symbol)) {
            return printErrorAndReturn("Enternal error in identiferState in scanner", ERROR_INTERNAL);
            // fprintf(stderr, "Enternal error in identiferState in scanner\n");
            // return(ERROR_INTERNAL);
            // return int error(str errorText, int error);
        }
        symbol = getc(stdin);
        identifierState(token);
    } else if (isspace(symbol)) {
        printf("Success, identifier is %s\n", token->content.str);
        token->type = T_ID;
        return NO_ERR; // success, return the identifier, clean buffer 
    } else {
        return printErrorAndReturn("Lexical error in identiferState in scanner", LEX_ERR);
        // return LEX_ERR;
    }
}

int underscoreState(token_t * token) {
    if (isalnum(symbol)) {
        if (!str_add_char(&token->content, symbol)){
            return printErrorAndReturn("Enternal error in underscoreState in scanner", ERROR_INTERNAL);
            // fprintf(stderr, "Enternal error in underscoreState in scanner\n");
            // return printErrorAndReturn(ERROR_INTERNAL);
        }
        symbol = getc(stdin);
        return identifierState(token);
    } else {
        return printErrorAndReturn("Lexical error in underscore state", LEX_ERR);
        // fprintf(stderr, "Lexical error in underscore state\n");
        // exit(LEX_ERR);
    }
}

int intState(token_t * token) {
    if (isdigit(symbol)) {
        if (!str_add_char(&token->content, symbol))
            return printErrorAndReturn("Enternal error in intState in scanner", ERROR_INTERNAL);
            // exit(ERROR_INTERNAL);
        symbol = getc(stdin);
        return intState(token);
    } else if (symbol == '.') {
        if (!str_add_char(&token->content, symbol))
            return printErrorAndReturn("Enternal error in intState in scanner", ERROR_INTERNAL);
            // exit(ERROR_INTERNAL);
        symbol = getc(stdin);
        if (isdigit(symbol)) {
            return floatState(token);
        } else {
            return printErrorAndReturn("Lexical error in intState in scanner", LEX_ERR);
            // exit(LEX_ERR);
        }
    } else if (symbol == 'E' || symbol == 'e') { // an Exponet part of float
        if (!str_add_char(&token->content, symbol))
            return printErrorAndReturn("Enternal error in intState in scanner", ERROR_INTERNAL);
            // exit(ERROR_INTERNAL);
        symbol = getc(stdin);
        if (symbol == '+' || symbol == '-' || isdigit(symbol)) {
            if (!str_add_char(&token->content, symbol))
                return printErrorAndReturn("Enternal error in intState in scanner", ERROR_INTERNAL);
                // exit(1);
            symbol = getc(stdin);
            return intExpState(token);
        } else {
            return printErrorAndReturn("Lexical error in intState in scanner", LEX_ERR);
            // exit(LEX_ERR);
        }
    } else if (isspace(symbol) || symbol == EOF) {
        printf("Success, int is %s\n", token->content.str);
        token->type = T_INT;
        return NO_ERR; // success, return the identifier, clean buffer 
    } else {
        return printErrorAndReturn("Lexical error in intState in scanner", LEX_ERR);
    }
}

int floatState(token_t * token) {
    if (isdigit(symbol)) {
        if (!str_add_char(&token->content, symbol))
            return printErrorAndReturn("Enternal error in floatState in scanner", ERROR_INTERNAL);
        symbol = getc(stdin);
        return floatState(token);
    } else if (symbol == 'E' || symbol == 'e') {
        if (!str_add_char(&token->content, symbol))
            return printErrorAndReturn("Enternal error in floatState in scanner", ERROR_INTERNAL);
        symbol = getc(stdin);
        if (symbol == '+' || symbol == '-' || isdigit(symbol)) {
            if (!str_add_char(&token->content, symbol))
                return printErrorAndReturn("Enternal error in floatState in scanner", ERROR_INTERNAL);
            symbol = getc(stdin);
            return floatExpState(token);
        } else {
            return printErrorAndReturn("Lexical error in floatState in scanner", LEX_ERR);
        }
    } else if (isspace(symbol) || symbol == EOF) {
        printf("Success, float is %s\n", token->content.str);
        token->type = T_FLOAT;
        return NO_ERR; // success, return the identifier, clean buffer 
    } else {
        return printErrorAndReturn("Lexical error 1 in floatState in scanner", LEX_ERR);
    }
}

int intExpState(token_t * token) {
    if (isdigit(symbol)) {
        if (!str_add_char(&token->content, symbol))
            return printErrorAndReturn("Enternal error in intExpState in scanner", ERROR_INTERNAL);
        symbol = getc(stdin);
        return floatState(token);
    } else if (isspace(symbol)) {
        printf("Success, int exp is %s\n", token->content.str);
        token->type = T_INT_EXP;
        return NO_ERR; // success, return the identifier, clean buffer 
    } else {
        return printErrorAndReturn("Lexical error in intExpState in scanner", LEX_ERR);
    }
}

int floatExpState(token_t * token) {
    if (isdigit(symbol)) {
        if (!str_add_char(&token->content, symbol))
            return printErrorAndReturn("Enternal error in floatExpState in scanner", ERROR_INTERNAL);
        symbol = getc(stdin);
        return floatState(token);
    } else if (isspace(symbol)) {
        printf("Success, float exp is %s\n", token->content.str);
        token->type = T_FLOAT_EXP;
        return NO_ERR; // success, return the identifier, clean buffer 
    } else {
        return printErrorAndReturn("Lexical error in floatExpState in scanner", LEX_ERR);
    }
}

int startState(token_t * token) {
    if (isspace(symbol)) { // White symbols
        symbol = getc(stdin);
        return startState(token);
    } else if (symbol == '_') { // underscore
        symbol = getc(stdin);
        return underscoreState(token);
    } else if (isalpha(symbol)) {
        if (!str_add_char(&token->content, symbol))
            return printErrorAndReturn("Enternal error in startState in scanner", ERROR_INTERNAL);
        symbol = getc(stdin);
        return identifierState(token);
    } else if (isdigit(symbol)) {
        if (!str_add_char(&token->content, symbol))
            return printErrorAndReturn("Enternal error in startState in scanner", ERROR_INTERNAL);
        symbol = getc(stdin);
        return intState(token);
    } else if (symbol == EOF) {
        token->type = 0;
        printf("Success, EOF is found\n");
    }
}

int getToken(token_t * token) {
    symbol = getc(stdin);
    int returnCode = startState(token);
    ungetc(symbol, stdin);
    str_clear(&token->content);
    char symbol = getc(stdin);
    int returnCode = startState(symbol, token);
    // ungetc(symbol, stdin);
    return returnCode;
}