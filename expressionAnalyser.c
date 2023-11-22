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

int literalSemCheck(precedenceStackNode_t *first,  precedenceStackNode_t *second) {
}

int checkExprSemantic(precedenceStackNode_t **top, int cnt, int *type, int rule) {
        
        precedenceStackNode_t *first = NULL;
        precedenceStackNode_t *second = NULL;
        precedenceStackNode_t *operator = NULL;
        
        if ( cnt == 3){
            precedenceStackNode_t *first = (*top)->next->next;
            precedenceStackNode_t *second = (*top);
            precedenceStackNode_t *operator = (*top);
        } else if ( cnt == 2) {
            precedenceStackNode_t *first = (*top)->next;
            precedenceStackNode_t *operator = (*top);
        } else {
            precedenceStackNode_t *first = (*top);
        }
        switch (rule) {
            case R_PLUS:
            case R_MINUS:
            case R_MUL:
            case R_DIV:
                if( (first->type > T_NIL && first->type < T_STRING) && (second->type > T_NIL && second->type < T_STRING)  ){
                    if (first->type == second->type) {
                        (*type) == first->type;
                        return NO_ERR;
                    }
                } else if (rule != R_DIV) {
                        if ( (first->type == ET_DOUBLE && second->type == ET_INT) || (first->type ==  ET_INT && second->type == ET_DOUBLE) ) {
                            if(first->lit && first->type == ET_INT) {
                                (*type) == ET_DOUBLE;
                                return NO_ERR;
                            } else if (second->lit && second->type == ET_INT) {
                                (*type) == ET_DOUBLE;
                                return NO_ERR;
                            }
                        } else if (rule == R_PLUS) {
                            if( first->type == ET_STRING && second->type == ET_STRING ){
                                (*type) == ET_STRING;
                                return NO_ERR;
                            }
                        }
                }
                return printErrorAndReturn("Semantic error occured while reducing rule", SEM_ERR_TYPE_COMPAT);
                break;
            case R_EQ:
            case R_NILCON:

            case R_UNAR:
                if (first->type >= T_DOUBLEN && first->type <= T_STRINGN) 
                    (*type) == first->type - 3;
                return NO_ERR;
                
            default:
                break;
        }

}

int reduceByRule(precedenceStackNode_t **top, int *cnt, int *type){
    int rule;
    if ((*cnt) == 1) {
        if((*top)->symbol == ES_ID){
            rule = R_ID;
            fprintf(stderr,"Id rule is parsed\n");
        } else {
            rule = R_ERROR;
        } 
    } else if ( (*cnt) == 2) { 
        if ((*top)->symbol == ES_EX){
            rule = R_UNAR;
        } else { 
                rule = R_ERROR;
        }
    } else if ((*cnt) == 3) {
        if (((*top)->next->next->symbol == ES_OP_PAR) && ((*top)->next->symbol == ES_NONTER) && ((*top)->symbol == ES_CL_PAR)) {
            rule =  R_PAR;
        } else if ( (*top)->next->next->symbol == ES_NONTER && (*top)->symbol == ES_NONTER ) {
            if ((*top)->next->symbol < R_UNAR){
                rule = (*top)->next->symbol;
            } else { 
                rule = R_ERROR;
            }
        }
    } else {
        rule = R_ERROR;
    }
    if (rule != R_ERROR ) {
        //checkExprSemantic(top, cnt, type, rule);
        for(int i = 0; i <= (*cnt); i++){
            prcStackPop(top);
        }
        prcStackPush(top, ES_NONTER, (*type));
        return NO_ERR;
    }

    return printErrorAndReturn("Syntax Error has occured in reduce by rule", SYNTAX_ERR);
}

int expAnalyse (token_t* tokenGlobal, token_t* tokenTmp, int *type) {
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
            prcStackPush(&stack, getSymbolFromToken(tokenExpr), tokenExpr->type);
            if (getTokenExpr(tokenGlobal, tokenTmp))
                result =  printErrorAndReturn("Lexical error has occured while expression analysing", LEX_ERR);
            break;
        case 'l':
            fprintf(stderr,"in case less with tokenExpr %d\nterminal on stack is %d\n",getSymbolFromToken(tokenExpr), prcStackGetTerminal(&stack)->symbol);
            prcStackPushAfter(&stackTerminal, ES_CATCH, ES_UNDEFINED);
            prcStackPush(&stack, getSymbolFromToken(tokenExpr),  tokenExpr->type);
            if (getTokenExpr(tokenGlobal, tokenTmp))
                result =  printErrorAndReturn("Lexical error has occured while expression analysing", LEX_ERR);
            break;

        case 'g':
            fprintf(stderr,"in case great with tokenExpr %d\nterminal on stack is %d\n",getSymbolFromToken(tokenExpr), prcStackGetTerminal(&stack)->symbol);
            if( ! (findCatch(&stack, &stackItemsCounter)) ) {
                result = reduceByRule(&stack, &stackItemsCounter, type);
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