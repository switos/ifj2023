#ifndef SCANNER_H
#define SCANNER_H

#include <ctype.h>
#include "token.h"
#include "error.h"

int identifierState(char symbol, token_t * token);
int underscoreState(char symbol, token_t * token);
int intState(char symbol, token_t * token);
int floatState(char symbol, token_t * token);
int floatExpState(char symbol, token_t * token);
int intExpState(char symbol, token_t * token);
int startState(char symbol, token_t * token);
int getToken(token_t * token);

#endif