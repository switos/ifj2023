#include "expressionAnalyser.h"

int expAnalyse (token_t* token) {
    precedenceStackNode_t* stack = NULL;
    prcStackPush(&stack, ES_END, ES_UNDEFINED);
    printf("%d \n", prcStackGetTerminal(&stack));   
}

