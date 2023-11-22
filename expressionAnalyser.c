#include "semantic.c"
token_t* tokenExpr = NULL;
int tokenFlag = 0;

void setTokenExpr(token_t* tokenGlobal, token_t* tokenTmp) {
    if (tokenTmp == NULL) {
        tokenExpr = tokenGlobal;
    } else {
        tokenExpr = tokenTmp;
        tokenFlag = 1;
    }
}

int getTokenExpr(token_t* tokenGlobal, token_t* tokenTmp) {
    if (tokenFlag == 1){
        tokenExpr = tokenGlobal;
        tokenFlag = 2;
        return 0;
    } else {
        return getToken(tokenGlobal);
    }
} 

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
            rule = R_ID;
            fprintf(stderr,"Id rule is parsed\n");
        } else {
            rule = R_ERROR;
        } 
    } else if ( (*cnt) == 2) { 
        fprintf(stderr,"I am Here\n");
        if ((*top)->symbol == ES_EX){
            rule = R_UNAR;
        }
    } else if ((*cnt) == 3) {
        if (((*top)->next->next->symbol == ES_OP_PAR) && ((*top)->next->symbol == ES_NONTER) && ((*top)->symbol == ES_CL_PAR)) {
            rule =  R_PAR;
        } else if ( (*top)->next->next->symbol == ES_NONTER && (*top)->symbol == ES_NONTER ) {
            if ((*top)->next->symbol < 11){
                rule = (*top)->next->symbol;
            } else { 
                rule = R_ERROR;
            }
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

int expAnalyse (token_t* tokenGlobal, token_t* tokenTmp) {
    token_t endToken;
    endToken.type = T_EOF; 
    precedenceStackNode_t* stack = NULL; 
    int stackItemsCounter = 0;
    prcStackPush(&stack, ES_END, ES_UNDEFINED);
    int result = 0;
    setTokenExpr(tokenGlobal, tokenTmp);
    do {
        precedenceStackNode_t* stackTerminal = prcStackGetTerminal(&stack);
        switch (precedenceTable[stackTerminal->symbol][getSymbolFromToken(tokenExpr)])
        {
        case '=':
            prcStackPush(&stack, getSymbolFromToken(tokenExpr), ES_UNDEFINED);
            if (getTokenExpr(tokenGlobal, tokenTmp))
                result =  printErrorAndReturn("Lexical error has occured while expression analysing", LEX_ERR);
            break;
        case 'l':
            fprintf(stderr,"in case less with tokenExpr %d\nterminal on stack is %d\n",getSymbolFromToken(tokenExpr), prcStackGetTerminal(&stack)->symbol);
            prcStackPushAfter(&stackTerminal, ES_CATCH, ES_UNDEFINED);
            prcStackPush(&stack, getSymbolFromToken(tokenExpr), ES_UNDEFINED);
            if (getTokenExpr(tokenGlobal, tokenTmp))
                result =  printErrorAndReturn("Lexical error has occured while expression analysing", LEX_ERR);
            break;

        case 'g':
            fprintf(stderr,"in case great with tokenExpr %d\nterminal on stack is %d\n",getSymbolFromToken(tokenExpr), prcStackGetTerminal(&stack)->symbol);
            if( ! (findCatch(&stack, &stackItemsCounter)) ) {
                result = reduceByRule(&stack, &stackItemsCounter);
            } else { 
                result = printErrorAndReturn("Syntax error has occured in expression analyser while findCatch\n", SYNTAX_ERR);
            }
            break;
        case 'e':
            result = printErrorAndReturn("Syntax error has occured in expression analyser\n", SYNTAX_ERR);
            break;
        case 'f':
            tokenExpr = &endToken;
            break;
        }
        fprintf(stderr,"%d\n%d\n",getSymbolFromToken(tokenExpr), prcStackGetTerminal(&stack)->symbol);
    } while (((getSymbolFromToken(tokenExpr) != ES_END) || (prcStackGetTerminal(&stack)->symbol != ES_END)) && result == 0 );
    prcStackFree(&stack);
    return result;
}