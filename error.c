/*
IFJ projekt 2023
xtiemn00: Vsevolod Tiemnohorov
xpetro27: Denys Petrovsyi
xbatur00: Illia Baturov
xshish02: Sviatoslav Shishnev
*/
#include "error.h"
#include <stdio.h>

int printErrorAndReturn(char * text, int code) {
    fprintf(stderr, "%s\n", text);
    return code;
}