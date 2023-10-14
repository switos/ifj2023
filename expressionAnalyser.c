#include "expressionAnalyser.h"

int findCatch(precedenceStackNode_t** top, int* count) {
    precedenceStackNode_t* current = (*top);
    int cnt = 0;
    while (current != NULL)
    {   
        if (current->symbol == ES_CATCH){
            (*count) = cnt; 
            return 0;
        }
        cnt++;
        current = current->next;
    }
    return 1;
}

int reduceByRule(precedenceStackNode_t** top, int* cnt){
    int rule;
    if ((*cnt) == 1) {
        if((*top)->type == ES_ID)
            rule == R_ID; 
    } else if ((*cnt) == 3) {
        /* code */
    } else rule = R_ERROR;
    for(int i = 0; i < (*cnt); i++){
        prcStackPop(top);
    }
    if (rule = R_ID) {
        prcStackPush(top, ES_NONTER, ES_UNDEFINED);
    }
    return NO_ERR;
}

int expAnalyse (token_t* token) {
    precedenceStackNode_t* stack = NULL; 
    int stackItemsCounter = 0;
    prcStackPush(&stack, ES_END, ES_UNDEFINED);
    int result;
    do {
        precedenceStackNode_t* stackTerminal = prcStackGetTerminal(&stack);
        switch (precedenceTable[stackTerminal->symbol][getSymbolFromToken(token)])
        {
        case '=':
            /* code */
            break;
        case 'l':
            prcStackPush(&stack, ES_CATCH, ES_UNDEFINED);
            prcStackPush(&stack, getSymbolFromToken(token), ES_UNDEFINED);
            break;
        case 'g':
            if( ! (findCatch(&stack, &stackItemsCounter)) )
                result = reduceByRule(&stack, &stackItemsCounter);
            else {
                prcStackFree(&stack);
                return ("Syntax error has occured in expression analyser while findCatch\n", SYNTAX_ERR);
            }
            break;
        case ' ':
            prcStackFree(&stack);
            return ("Syntax error has occured in expression analyser\n", SYNTAX_ERR);
            break;
        }
        if (getToken(token)) {
            prcStackFree(&stack);
            return printErrorAndReturn("Lexical error has occured while expression analysing", LEX_ERR);
        }
        printf("%d\n%d\n",getSymbolFromToken(token), prcStackGetTerminal(&stack)->symbol);
    } while (!(getSymbolFromToken(token) == ES_END) || !(prcStackGetTerminal(&stack)->symbol == ES_END));
    prcStackFree(&stack);
    return result;
}