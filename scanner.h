/*
IFJ projekt 2023
xtiemn00: Vsevolod Tiemnohorov
xpetro27: Denys Petrovsyi
xbatur00: Illia Baturov
xshish02: Sviatoslav Shishnev
*/
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
int stringState(token_t * token);
int twoQuotesStringState(token_t * token);
int multilineStringState(token_t * token);
int oneQuoteState(token_t * token);
void endOfMultilineStringDetector(char * temp);
bool isKeyword(token_t * token);
bool isType(token_t * token, bool qFlag);
bool isAcc();
bool isCommi();
bool isOper(token_t * token);
int escapeAllowed(token_t * token);
int hex(token_t * token);

#endif