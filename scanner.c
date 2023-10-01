#include "scanner.h"

string tokenContent;
token_t token;
char symbol;

/**
 * @brief function for identifier state
 * 
 * @param token 
 * @return int 
 */
int identifierState(token_t * token) {
    if (isalnum(symbol) || symbol == '_') { // _ or a-Z or 1-9
        if (!str_add_char(&token->content, symbol)) {
            return printErrorAndReturn("Enternal error in identiferState in scanner", ERROR_INTERNAL);
        }
        symbol = getc(stdin);
        return identifierState(token);
    } else if (isspace(symbol) || symbol == EOF) { // white symbol or EOF
        printf("Success, identifier is %s\n", token->content.str);
        token->type = T_ID;
        return NO_ERR; // success, return the identifier, clean buffer 
    } else {
        return printErrorAndReturn("Lexical error in identiferState in scanner", LEX_ERR);
    }
}

/**
 * @brief function for underscore state
 * 
 * @param token 
 * @return int 
 */
int underscoreState(token_t * token) {
    if (isalnum(symbol)) { // a-Z or 1-9 allowed
        if (!str_add_char(&token->content, symbol)){
            return printErrorAndReturn("Enternal error in underscoreState in scanner", ERROR_INTERNAL);
        }
        symbol = getc(stdin);
        return identifierState(token);
    } else {
        return printErrorAndReturn("Lexical error in underscore state", LEX_ERR);
    }
}

/**
 * @brief function for int state
 * 
 * @param token 
 * @return int 
 */
int intState(token_t * token) {
    if (isdigit(symbol)) { // integer
        if (!str_add_char(&token->content, symbol))
            return printErrorAndReturn("Enternal error in intState in scanner", ERROR_INTERNAL);
        symbol = getc(stdin);
        return intState(token);
    } else if (symbol == '.') { // float
        if (!str_add_char(&token->content, symbol))
            return printErrorAndReturn("Enternal error in intState in scanner", ERROR_INTERNAL);
        symbol = getc(stdin);
        if (isdigit(symbol)) {
            return floatState(token);
        } else {
            return printErrorAndReturn("Lexical error in intState in scanner", LEX_ERR);
        }
    } else if (symbol == 'E' || symbol == 'e') { // an Exponet part of integer
        if (!str_add_char(&token->content, symbol))
            return printErrorAndReturn("Enternal error in intState in scanner", ERROR_INTERNAL);
        symbol = getc(stdin);
        if (symbol == '+' || symbol == '-' || isdigit(symbol)) {
            if (!str_add_char(&token->content, symbol))
                return printErrorAndReturn("Enternal error in intState in scanner", ERROR_INTERNAL);
            symbol = getc(stdin);
            return intExpState(token);
        } else {
            return printErrorAndReturn("Lexical error in intState in scanner", LEX_ERR);
        }
    } else if (isspace(symbol) || symbol == EOF) { // EOF or white symbol
        printf("Success, int is %s\n", token->content.str);
        token->type = T_INT;
        return NO_ERR; // success, return the identifier, clean buffer 
    } else {
        return printErrorAndReturn("Lexical error in intState in scanner", LEX_ERR);
    }
}

/**
 * @brief function for float state
 * 
 * @param token 
 * @return int 
 */
int floatState(token_t * token) {
    if (isdigit(symbol)) { // float
        if (!str_add_char(&token->content, symbol))
            return printErrorAndReturn("Enternal error in floatState in scanner", ERROR_INTERNAL);
        symbol = getc(stdin);
        return floatState(token);
    } else if (symbol == 'E' || symbol == 'e') { // an Exponet part of float
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
    } else if (isspace(symbol) || symbol == EOF) { // EOF or white symbol
        printf("Success, float is %s\n", token->content.str);
        token->type = T_FLOAT;
        return NO_ERR; // success, return the identifier, clean buffer 
    } else {
        return printErrorAndReturn("Lexical error in floatState in scanner", LEX_ERR);
    }
}

/**
 * @brief function for exponencial int state
 * 
 * @param token 
 * @return int 
 */
int intExpState(token_t * token) {
    if (isdigit(symbol)) {
        if (!str_add_char(&token->content, symbol))
            return printErrorAndReturn("Enternal error in intExpState in scanner", ERROR_INTERNAL);
        symbol = getc(stdin);
        return intExpState(token);
    } else if (isspace(symbol) || symbol == EOF) {
        printf("Success, int exp is %s\n", token->content.str);
        token->type = T_INT_EXP;
        return NO_ERR; // success, return the identifier, clean buffer 
    } else {
        return printErrorAndReturn("Lexical error in intExpState in scanner", LEX_ERR);
    }
}

/**
 * @brief function for exponencial float state
 * 
 * @param token 
 * @return int 
 */
int floatExpState(token_t * token) {
    if (isdigit(symbol)) {
        if (!str_add_char(&token->content, symbol))
            return printErrorAndReturn("Enternal error in floatExpState in scanner", ERROR_INTERNAL);
        symbol = getc(stdin);
        return floatExpState(token);
    } else if (isspace(symbol) || symbol == EOF) {
        printf("Success, float exp is %s\n", token->content.str);
        token->type = T_FLOAT_EXP;
        return NO_ERR; // success, return the identifier, clean buffer 
    } else {
        return printErrorAndReturn("Lexical error in floatExpState in scanner", LEX_ERR);
    }
}

/**
 * @brief function that checks if we have another quote after start (first) quote
 * 
 * @param token 
 * @return int 
 */
int oneQuoteState(token_t * token) {
    if (symbol == '\"') {
        symbol = getc(stdin);
        return twoQuotesStringState(token);
    } else if (symbol > 31) {
        return stringState(token);
    }
}

/**
 * @brief function for string state
 * 
 * @param token 
 * @return int 
 */
int stringState(token_t * token) {
    if (symbol > 31 && symbol != '\"') {
        if (!str_add_char(&token->content, symbol)) {
            return printErrorAndReturn("Enternal error in stringState in scanner", ERROR_INTERNAL);
        }
        symbol = getc(stdin);
        return stringState(token);
    } else if (symbol == '\"') {
        symbol = getc(stdin);
        if (isspace(symbol) || symbol == EOF) {
            printf("Success, string content is %s\n", token->content.str);
            token->type = T_STRING;
            return NO_ERR; // success, return the string, clean buffer 
        } else {
            return printErrorAndReturn("Lexical error in stringState in scanner after final quote", LEX_ERR);
        }
    } else {
        return printErrorAndReturn("Lexical error in stringState in scanner during parse", LEX_ERR);
    }
}

/**
 * @brief function that checks if we have multiline string or empty string
 * 
 * @param token 
 * @return int 
 */
int twoQuotesStringState(token_t * token) {
    if (symbol == EOF || isspace(symbol)) {
        printf("Success, empty string detected\n");
        token->type = T_STRING;
        return NO_ERR; // success, return the empty string, clean buffer 
    } else if (symbol == '\"') {
        symbol = getc(stdin);
        if (symbol == '\n') { // we should check if it's multiline string (starts with 3 quotes and new line)
            symbol = getc(stdin);
            return multilineStringState(token);
        } else {
            return printErrorAndReturn("Lexical error in twoQuotesStringState in scanner after starting 3 quotes", LEX_ERR);
        }
    }
}

/**
 * @brief helper function to make sure this is the end of multiline string
 * 
 * @return char* saved content of original string (in case if it's not the end)
 */
char* endOfMultilineStringDetector() {
    char returnValue[6];
    returnValue[0] = symbol;
    for (int i = 0; i < 3; i++) {
        symbol = getc(stdin);
        returnValue[i+1] = symbol;
        if (symbol != '\"') { // if symbol is not quote, then we can put value of parsed part into main string token
            returnValue[i+2] = '\0';
            return returnValue;
        }
    }
    returnValue[0] = '\0';
    return returnValue;
}

/**
 * @brief function for multiline string state
 * 
 * @param token 
 * @return int 
 */
int multilineStringState(token_t * token) {
    char temp[6];   // we need this variable to store content of the original string from stream in case of repeating quotes 
                    // (either final of string or error if 3 quotes are without '\n')
    temp[0] = '\0';
    if (symbol > 31) { // get content of multiline string, excluding '\n' because it might be before the final three quotes
        if (symbol == '\"') { // if quote detected, we need to check if we have 3 quotes in order without new line, this means lexical error
            temp[0] = *endOfMultilineStringDetector();
            if (temp[0] == '\"' && temp[1] == '\"' && temp[2] == '\"') {
                return printErrorAndReturn("Lexical error in multilineStringState in scanner", LEX_ERR);
            } else {
                if (!str_add_more_chars(&token->content, temp))
                    return printErrorAndReturn("Enternal error in multilineStringState in scanner during copy", ERROR_INTERNAL);
            }
        }
        if (!str_add_char(&token->content, symbol))
            return printErrorAndReturn("Enternal error in multilineStringState in scanner", ERROR_INTERNAL);
        symbol = getc(stdin);
        return multilineStringState(token);
    } else if (symbol == '\n') { // now we should check if it's the end of the multiline string or not
        temp[0] = *endOfMultilineStringDetector();
        if (temp[0] != '\0') { // check if it were not final 3 quotes
            if (!str_add_more_chars(&token->content, temp))
                return printErrorAndReturn("Enternal error in multilineStringState in scanner during copy", ERROR_INTERNAL);
            return multilineStringState(token);
        } else {
            symbol = getc(stdin);
            if (symbol == '\n' || symbol == EOF) {
                printf("Success, multistring content is @@%s@@\n", token->content.str);
                token->type = T_STRING;
                return NO_ERR; // success, return the string, clean buffer 
            } else {
                return printErrorAndReturn("Lexical error in multilineStringState in scanner after final 3 quotes", LEX_ERR);
            }
        }
    }
}

/**
 * @brief all begins here
 * 
 * @param token 
 * @return int 
 */
int startState(token_t * token) {
    if (isspace(symbol)) { // White symbols
        symbol = getc(stdin);
        return startState(token);
    } else if (symbol == '_') { // underscore
        symbol = getc(stdin);
        return underscoreState(token);
    } else if (isalpha(symbol)) { // no underscore
        if (!str_add_char(&token->content, symbol))
            return printErrorAndReturn("Enternal error in startState in scanner", ERROR_INTERNAL);
        symbol = getc(stdin);
        return identifierState(token);
    } else if (isdigit(symbol)) { // number
        if (!str_add_char(&token->content, symbol))
            return printErrorAndReturn("Enternal error in startState in scanner", ERROR_INTERNAL);
        symbol = getc(stdin);
        return intState(token);
    } else if (symbol == '\"') {
        symbol = getc(stdin);
        return oneQuoteState(token);
    } else if (symbol == EOF) {
        token->type = 0;
        printf("Success, EOF is found\n");
    }
}

/**
 * @brief Get the Token object
 * 
 * @param token 
 * @return int 
 */
int getToken(token_t * token) {
    symbol = getc(stdin);
    int returnCode = startState(token);
    ungetc(symbol, stdin);
    str_clear(&token->content);
    return returnCode;
}