#include "error.h"
#include <stdio.h>

int printErrorAndReturn(char * text, int code) {
    if (text == NULL) {
        fprintf(stderr, "Сева бракодел\n");
        return code;
    }
    
    fprintf(stderr, "%s\n", text);
    return code;
}