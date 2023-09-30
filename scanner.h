#ifndef SCANNER_H
#define SCANNER_H

#include <ctype.h>
#include "token.h"
#include "error.h"

int identifierState(token_t * token);
int underscoreState(token_t * token);
int intState(token_t * token);
int floatState(token_t * token);
int floatExpState(token_t * token);
int intExpState(token_t * token);
int startState(token_t * token);
int getToken(token_t * token);

#endif