#include "string.h"


int identifierState(char symbol) {
    if ((symbol>64&&symbol<91) || (symbol>96 && symbol<123) || (symbol>47 && symbol<58) || (symbol == 95) ) {
        symbol = getc();
        identifierState(symbol);
    } else {
        return 0; //success, return the identifier, clean buffer 
    }
}

int underscroreState(char symbol) {
    if ((symbol>64&&symbol<91) || (symbol>96 && symbol<123) || (symbol>47 && symbol<58)) {
            symbol = getc();
            identifierState(symbol);
    } else {
        return 1; //error 
    }
}

int start_state( char symbol) {
    if(symbol == "_") {
            symbol = getc();
            underscoreState();
    } else if ((symbol>64&&symbol<91) || (symbol>96 && symbol<123)) {
            symbol = getc();
            identifierState(symbol);
    }
    
}

int get_token() {
    char symbol = getc();
}