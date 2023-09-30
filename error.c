#include "error.h"
#include <stdio.h>

int printErrorAndReturn(char * text, int code) {
    fprintf(stderr + '\n', text);
    return code;
}