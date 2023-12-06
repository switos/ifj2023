/*
IFJ projekt 2023
xtiemn00: Vsevolod Tiemnohorov
xpetro27: Denys Petrovsyi
xbatur00: Illia Baturov
xshish02: Sviatoslav Shishnev
*/
#include "scanner.h"

string tokenContent;

char symbol;

/**
 * @brief checks if lesem is type
 * 
 * @param token 
 * @return true 
 * @return false 
 */
bool isType(token_t * token, bool qFlag) {
    if (!str_cmp_const_str(&token->content, "Double")) {
        if (qFlag) {
            token->type = T_DOUBLEN;
        } else {
            token->type = T_DOUBLE;
        }
    } else if (!str_cmp_const_str(&token->content, "String")) {
        if (qFlag) {
            token->type = T_STRINGN;
        } else {
            token->type = T_STRING;
        }
    }  else if (!str_cmp_const_str(&token->content, "Int")) {
        if (qFlag) {
            token->type = T_INTN;
        } else {
            token->type = T_INT;
        }
    } else {
        return false;
    }
    return true;
}

/**
 * @brief checks if lexem is keyword or type
 * 
 * @param token 
 * @return true 
 * @return false 
 */
bool isKeyword(token_t * token) {
    if (isType(token, false)) {
        return true;
    }

    if (!str_cmp_const_str(&token->content, "let")) {
        token->type = T_LET;
    } else if (!str_cmp_const_str(&token->content, "var")) {
        token->type = T_VAR;
    } else if (!str_cmp_const_str(&token->content, "else")) {
        token->type = T_ELSE;
    } else if (!str_cmp_const_str(&token->content, "func")) {
        token->type = T_FUNC;
    } else if (!str_cmp_const_str(&token->content, "if")) {
        token->type = T_IF;
    } else if (!str_cmp_const_str(&token->content, "nil")) {
        token->type = T_NIL;
    } else if (!str_cmp_const_str(&token->content, "return")) {
        token->type = T_RETURN;
    } else if (!str_cmp_const_str(&token->content, "while")) {
        token->type = T_WHILE;
    } else {
        return false;
    }
    return true;
}

/**
 * @brief checks if it's comment
 * 
 * @return true 
 * @return false 
 */
bool isCommi() {
    if (symbol == '/') {
        symbol = getc(stdin);
        if (symbol == '/') {
            while((symbol = getc(stdin)) != '\n' && symbol != EOF);
            return true;
        } else if (symbol == '*') {
            int nestedCounter = 1;
            while(nestedCounter) {
                symbol = getc(stdin);
                if (symbol == '*') {
                    symbol = getc(stdin);
                    if (symbol == '/') {
                        nestedCounter--;
                        if (!nestedCounter) {
                            symbol = getc(stdin);
                        }
                    } else {
                        ungetc(symbol, stdin);
                    }
                } else if (symbol == '/') {
                    symbol = getc(stdin);
                    if (symbol == '*') {
                        nestedCounter++;
                    } else {
                        ungetc(symbol, stdin);
                    }
                }
                if (symbol == EOF) {
                    return false;
                }
            }
            return true;
        } else {
            ungetc('/', stdin);
            symbol = getc(stdin);
        }
    }
    return false;
}

/**
 * @brief checks if symbol could be after lexem
 * 
 * @return true 
 * @return false 
 */
bool isAcc() {
    if (isspace(symbol) || symbol == EOF) {
        return true;
    } else if (symbol == '/') {
        return isCommi();
    }
    return false;
}

/**
 * @brief checks if symbol is operator
 * 
 * @return true 
 * @return false 
 */
bool isOper(token_t * token) {
    if (symbol == '{') {
        fprintf(stderr, "Success, oper is {\n");
        token->type = T_OP_BRACE;
    } else if (symbol == '}') {
        fprintf(stderr, "Success, oper is }\n");
        token->type = T_CL_BRACE;
    } else if (symbol == '(') {
        fprintf(stderr, "Success, oper is (\n");
        token->type = T_OP_PAR;
    } else if (symbol == ')') {
        fprintf(stderr, "Success, oper is )\n");
        token->type = T_CL_PAR;
    } else if (symbol == '*') {
        fprintf(stderr, "Success, oper is *\n");
        token->type = T_ASTER;
    } else if (symbol == '/') {
        fprintf(stderr, "Success, oper is /\n");
        token->type = T_SLASH;
    } else if (symbol == '+') {
        fprintf(stderr, "Success, oper is +\n");
        token->type = T_PLUS;
    } else if (symbol == '-') {
        if ((symbol = getc(stdin)) != '>') {
            fprintf(stderr, "Success, oper is -\n");
            ungetc(symbol, stdin);
            token->type = T_MINUS;
        } else {
            fprintf(stderr, "Success, oper is ->\n");
            token->type = T_ARROW;
        }
    } else if (symbol == '=') {
        if ((symbol = getc(stdin)) != '=') {
            fprintf(stderr, "Success, oper is =\n");
            ungetc(symbol, stdin);
            token->type = T_ASSING;
        } else {
            fprintf(stderr, "Success, oper is ==\n");
            token->type = T_EQUAL;
        }
    } else if (symbol == '!') {
        if ((symbol = getc(stdin)) != '=') {
            fprintf(stderr, "Success, oper is !\n");
            ungetc(symbol, stdin);
            token->type = T_EXCL;
        } else {
            fprintf(stderr, "Success, oper is !=\n");
            token->type = T_NOT_EQUAL;
        }
    } else if (symbol == '<') {
        if ((symbol = getc(stdin)) != '=') {
            fprintf(stderr, "Success, oper is <\n");
            ungetc(symbol, stdin);
            token->type = T_LESS;
        } else {
            fprintf(stderr, "Success, oper is <=\n");
            token->type = T_LESS_EQUAL;
        }
    } else if (symbol == '>') {
        if ((symbol = getc(stdin)) != '=') {
            fprintf(stderr, "Success, oper is >\n");
            ungetc(symbol, stdin);
            token->type = T_GREATER;
        } else {
            fprintf(stderr, "Success, oper is >=\n");
            token->type = T_GREATER_EQUAL;
        }
    } else if (symbol == '?') {
        if ((symbol = getc(stdin)) == '?') {
            fprintf(stderr, "Success, oper is ??\n");
            token->type = T_NIL_COAL;
        } else {
            return false;
        }
    } else if (symbol == ':') {
        fprintf(stderr, "Success, oper is :\n");
        token->type = T_COLON;
    } else if (symbol == ',') {
        fprintf(stderr, "Success, oper is ,\n");
        token->type = T_COMMA;
    } else {
        return false;
    }
    return true;
}

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
    } else if (symbol == '?') {
        if (isType(token, true)) {
            symbol = getc(stdin);
            if (isAcc()) {
                return NO_ERR;
            } else {
                return printErrorAndReturn("Lexical error in identiferState in scanner", LEX_ERR);
            }
        } else {
            return printErrorAndReturn("Lexical error in identiferState in scanner", LEX_ERR);
        }
    } else {
        fprintf(stderr, "Success, identifier is %s\n", token->content.str);
        if (!isKeyword(token)){
            token->type = T_ID;
        } else {
            fprintf(stderr, "Success, it is a keyword\n");
        }
        return NO_ERR; // success, return the identifier, clean buffer 
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
    } else if (symbol == 'E' || symbol == 'e') { // an Exponent part of integer
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
    } else {
        if (isalpha(symbol)) {
            return printErrorAndReturn("Lexical error (alpha after) in intState in scanner", LEX_ERR);
        } else {
            fprintf(stderr, "Success, int is %s\n", token->content.str);
            token->type = T_INT_LIT;
            return NO_ERR; // success, return the identifier, clean buffer 
        }
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
    } 
    // else if (isspace(symbol) || symbol == EOF) { // EOF or white symbol
    //     fprintf(stderr, "Success, float is %s\n", token->content.str);
    //     token->type = T_FLOAT_LIT;
    //     return NO_ERR; // success, return the identifier, clean buffer 
    // } 
    else {
        fprintf(stderr, "Success, float is %s\n", token->content.str);
        token->type = T_FLOAT_LIT;
        return NO_ERR; // success, return the identifier, clean buffer 
        // return printErrorAndReturn("Lexical error in floatState in scanner", LEX_ERR);
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
    } 
    // else if (isspace(symbol) || symbol == EOF) {
    //     fprintf(stderr, "Success, int exp is %s\n", token->content.str);
    //     token->type = T_FLOAT_LIT;
    //     return NO_ERR; // success, return the identifier, clean buffer 
    // } 
    else {
        fprintf(stderr, "Success, int exp is %s\n", token->content.str);
        token->type = T_FLOAT_LIT;
        return NO_ERR; // success, return the identifier, clean buffer 
        // return printErrorAndReturn("Lexical error in intExpState in scanner", LEX_ERR);
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
    } 
    // else if (isspace(symbol) || symbol == EOF) {
    //     fprintf(stderr, "Success, float exp is %s\n", token->content.str);
    //     token->type = T_FLOAT_LIT;
    //     return NO_ERR; // success, return the identifier, clean buffer 
    // } 
    else {
        fprintf(stderr, "Success, float exp is %s\n", token->content.str);
        token->type = T_FLOAT_LIT;
        return NO_ERR; // success, return the identifier, clean buffer 
        // return printErrorAndReturn("Lexical error in floatExpState in scanner", LEX_ERR);
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
        // fprintf(stderr, "second\n");
        symbol = getc(stdin);
        return twoQuotesStringState(token);
    } else if (symbol > 31) {
        return stringState(token);
    }
    return printErrorAndReturn("Lexical error in oneQuoteState", LEX_ERR);
}

int escapeAllowed(token_t * token) {
    if (symbol == '\"') {
        if (!str_add_char(&token->content, '\"')) {
            return printErrorAndReturn("Enternal error in escapeAllowed in scanner", ERROR_INTERNAL);
        }
    } else if (symbol == 'n') {
        if (!str_add_char(&token->content, '\n')) {
            return printErrorAndReturn("Enternal error in escapeAllowed in scanner", ERROR_INTERNAL);
        }
    } else if (symbol == 'r') {
        if (!str_add_char(&token->content, '\r')) {
            return printErrorAndReturn("Enternal error in escapeAllowed in scanner", ERROR_INTERNAL);
        }
    } else if (symbol == 't') {
        if (!str_add_char(&token->content, '\t')) {
            return printErrorAndReturn("Enternal error in escapeAllowed in scanner", ERROR_INTERNAL);
        }
    }  else if (symbol == '\\') {
        if (!str_add_char(&token->content, '\\')) {
            return printErrorAndReturn("Enternal error in escapeAllowed in scanner", ERROR_INTERNAL);
        }
    } else if (symbol == 'u') {
        if (!str_add_more_chars(&token->content, "\\u")) {
            return printErrorAndReturn("Enternal error in escapeAllowed in scanner", ERROR_INTERNAL);
        }
        symbol = getc(stdin);
        return hex(token);// \u{dd}
    } else {
        return printErrorAndReturn("Lexical error in escapeAllowed in scanner", LEX_ERR);
    }
    return NO_ERR;
}

int hex(token_t * token) {
    if (symbol == '{') {
        if (!str_add_char(&token->content, symbol)) {
            return printErrorAndReturn("Enternal error in escapeAllowed in scanner", ERROR_INTERNAL);
        }
        symbol = getc(stdin);
        while (isxdigit(symbol)) {
            if (!str_add_char(&token->content, symbol)) {
                return printErrorAndReturn("Enternal error in escapeAllowed in scanner", ERROR_INTERNAL);
            }
            symbol = getc(stdin);
            if (symbol == '}') {
                if (!str_add_char(&token->content, symbol)) {
                    return printErrorAndReturn("Enternal error in escapeAllowed in scanner", ERROR_INTERNAL);
                }
                // fprintf(stderr, "first symbol : %c\n", symbol);
                // symbol = getc(stdin);
                // fprintf(stderr, "second symbol : %c\n", symbol);
                return NO_ERR;
            }
        }
    }
    return printErrorAndReturn("Lexical error in hex in scanner", LEX_ERR);
}

/**
 * @brief function for string state
 * 
 * @param token 
 * @return int 
 */
int stringState(token_t * token) {
    if (symbol > 31 && symbol != '\"') {
        if (symbol == '\\') {
            symbol = getc(stdin);
            if (escapeAllowed(token)) {
                return printErrorAndReturn("Lexical error in stringState in scanner in escape", LEX_ERR);
            }
        } else {
            if (!str_add_char(&token->content, symbol)) {
                return printErrorAndReturn("Enternal error in stringState in scanner", ERROR_INTERNAL);
            }
        }
        symbol = getc(stdin);
        return stringState(token);
    } else if (symbol == '\"') {
        symbol = getc(stdin);
        fprintf(stderr, "Success, string content is @@%s@@\n", token->content.str);
        token->type = T_STRING_LIT;
        return NO_ERR; // success, return the string, clean buffer 
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
        fprintf(stderr, "Success, empty string detected\n");
        token->type = T_STRING_LIT;
        return NO_ERR; // success, return the empty string, clean buffer 
    } else if (symbol == '\"') {
        // fprintf(stderr, "third\n");
        symbol = getc(stdin);
        if (symbol == '\n') { // we should check if it's multiline string (starts with 3 quotes and new line)
            // fprintf(stderr, "fourth\n");
            symbol = getc(stdin);
            return multilineStringState(token);
        } else {
            return printErrorAndReturn("Lexical error in twoQuotesStringState in scanner after starting 3 quotes", LEX_ERR);
        }
    }
    return printErrorAndReturn("Lexical error in twoQuotesStringState in scanner", LEX_ERR);
}


/**
 * @brief helper function to make sure this is the end of multiline string
 * 
 * @return char* saves content of original string (in case if it's not the end)
 */
void endOfMultilineStringDetector(char* temp) {
    // fprintf(stderr, "fifth\n");
    temp[0] = symbol;
    for (int i = 0; i < 3; i++) {
        symbol = getc(stdin);
        while(isspace(symbol)) {
            // fprintf(stderr, "zigagiga\n");
            symbol = getc(stdin);
        }
        // fprintf(stderr, "cur2 symbol : %c\n", symbol);
        temp[i+1] = symbol;
        if (symbol != '\"') { // if symbol is not quote, then we can put value of parsed part into main string token
            // fprintf(stderr, "sixth\n");
            if (symbol == '\n') {
                ungetc(symbol, stdin);
                temp[i+1] = '\0';
                symbol = getc(stdin);
            } else {
                temp[i+2] = '\0';
                symbol = getc(stdin);
            }
            return;
        }
    }
    temp[0] = '\0';
    return;
}

/**
 * @brief function for multiline string state
 * 
 * @param token 
 * @return int 
 */
int multilineStringState(token_t * token) {
    fprintf(stderr, "cur symbol : %c\n", symbol);
    char temp[6];   // we need this variable to store content of the original string from stream in case of repeating quotes 
                    // (either final of string or error if 3 quotes are without '\n')
    temp[0] = '\0';
    if (symbol > 31) { // get content of multiline string, excluding '\n' because it might be before the final three quotes
        if (symbol == '\"') { // if quote detected, we need to check if we have 3 quotes in order without new line, this means lexical error
            endOfMultilineStringDetector(temp);
            if (temp[0] == '\"' && temp[1] == '\"' && temp[2] == '\"') {
                return printErrorAndReturn("First lexical error in multilineStringState in scanner", LEX_ERR);
            } else {
                if (!str_add_more_chars(&token->content, temp))
                    return printErrorAndReturn("Enternal error in multilineStringState in scanner during copy", ERROR_INTERNAL);
            }
        } else {
            if (!str_add_char(&token->content, symbol))
                return printErrorAndReturn("Enternal error in multilineStringState in scanner", ERROR_INTERNAL);
            symbol = getc(stdin);
        }
        return multilineStringState(token);
    } else if (symbol == '\n') { // now we should check if it's the end of the multiline string or not
        endOfMultilineStringDetector(temp);
        if (temp[0] != '\0') { // check if it were not final 3 quotes
            if (!str_add_more_chars(&token->content, temp))
                return printErrorAndReturn("Enternal error in multilineStringState in scanner during copy", ERROR_INTERNAL);
            return multilineStringState(token);
        } else {
            // fprintf(stderr, "aboba\n");
            symbol = getc(stdin);
            if (isAcc()) {
                fprintf(stderr, "Success, multistring content is @@%s@@\n", token->content.str);
                token->type = T_STRING_LIT;
                return NO_ERR; // success, return the string, clean buffer 
            } else {
                return printErrorAndReturn("Second lexical error in multilineStringState in scanner after final 3 quotes", LEX_ERR);
            }
        }
    }
    return printErrorAndReturn("Second lexical error in multilineStringState in scanner after final 3 quotes", LEX_ERR);
}

/**
 * @brief all begins here
 * 
 * @param token 
 * @return int 
 */
int startState(token_t * token) {
    if (isspace(symbol)) { // White symbols
        if (symbol == '\n') {
            tFlagS(token);
            symbol = getc(stdin);
            return startState(token);
        }
        symbol = getc(stdin);
        return startState(token);
    } else if (symbol == '_') { // underscore
        if (!str_add_char(&token->content, symbol))
            return printErrorAndReturn("Enternal error in startState in scanner", ERROR_INTERNAL);
        symbol = getc(stdin);
        if (isalnum(symbol)) 
            return underscoreState(token);
        else {
            token->type = T_UNDER;
            return NO_ERR;
        }
             
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
        // fprintf(stderr, "first\n");
        symbol = getc(stdin);
        return oneQuoteState(token);
    } else if (symbol == EOF) {
        token->type = T_EOF;
        fprintf(stderr, "Success, EOF is found\n");
    } else if (symbol == '/') {
        if (isCommi()) {
            return startState(token);
        } else {
            if (isOper(token)) {
                symbol = getc(stdin);
                return NO_ERR;
            } else {
                return printErrorAndReturn("Lexical error in startState in scanner (commi)", LEX_ERR);
            }
        }
    } else {
        if (isOper(token) || isAcc()) {
            symbol = getc(stdin);
            return NO_ERR;
        }
        // fprintf(stderr, "last symbol : %c\n", symbol);
        return printErrorAndReturn("Lexical error in startState in scanner (last else)", LEX_ERR);
    }
    return NO_ERR;
}

/**
 * @brief Get the Token object
 * 
 * @param token 
 * @return int 
 */
int getToken(token_t * token) {
    str_clear(&token->content);
    tFlagR(token);
    symbol = getc(stdin);
    int returnCode = startState(token);
    ungetc(symbol, stdin);
    return returnCode;
}