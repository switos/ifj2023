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
        if((*top)->symbol == ES_ID){
            rule == R_ID;
            printf("Id rule is parsed\n");
        } else {
            rule = R_ERROR;
        }
    } else if ((*cnt) == 3) {
        if ((*top)->next->symbol == ES_PLUS){
            rule = R_PLUS;
            printf("Plus rule is parsed\n");
        } else if ((*top)->next->symbol == ES_MINUS) {
            rule = R_MINUS;
        } else if  ((*top)->next->symbol == ES_MUL) {
            rule = R_MUL;
        } else if  ((*top)->next->symbol == ES_DIV) {
            rule = R_DIV;
        }
        else { 
            rule = R_ERROR;
        }
    } else {
        rule = R_ERROR;
    }
    if (rule != R_ERROR ) {
        for(int i = 0; i <= (*cnt); i++){
            prcStackPop(top);
        }
        prcStackPush(top, ES_NONTER, ES_UNDEFINED);
        return NO_ERR;
    }

    return printErrorAndReturn("Syntax Error has occured in reduce by rule", SYNTAX_ERR);
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
            printf("in case less with token %d\nterminal on stack is %d\n",getSymbolFromToken(token), prcStackGetTerminal(&stack)->symbol);
            prcStackPushAfter(&stackTerminal, ES_CATCH, ES_UNDEFINED);
            prcStackPush(&stack, getSymbolFromToken(token), ES_UNDEFINED);
            if (getToken(token))
                result =  printErrorAndReturn("Lexical error has occured while expression analysing", LEX_ERR);
            break;

        case 'g':
            printf("in case great with token %d\nterminal on stack is %d\n",getSymbolFromToken(token), prcStackGetTerminal(&stack)->symbol);
            if( ! (findCatch(&stack, &stackItemsCounter)) ) {
                result = reduceByRule(&stack, &stackItemsCounter);
            } else { 
                result = printErrorAndReturn("Syntax error has occured in expression analyser while findCatch\n", SYNTAX_ERR);
            }
            break;
        case ' ':
            result = printErrorAndReturn("Syntax error has occured in expression analyser\n", SYNTAX_ERR);
            break;
        }
        printf("%d\n%d\n",getSymbolFromToken(token), prcStackGetTerminal(&stack)->symbol);
    } while (((getSymbolFromToken(token) != ES_END) || (prcStackGetTerminal(&stack)->symbol != ES_END)) && result == 0 );
    prcStackFree(&stack);
    return result;
}