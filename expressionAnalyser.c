/*
IFJ projekt 2023
xtiemn00: Vsevolod Tiemnohorov
xpetro27: Denys Petrovsyi
xbatur00: Illia Baturov
xshish02: Sviatoslav Shishnev
*/
#include "semantic.c"
token_t* tokenExpr = NULL;
int tokenFlag = 0;


int setTokenExpr(token_t* tokenGlobal, token_t* tokenTmp, symtable_stack_t *symStack) {
    if (tokenTmp == NULL) {
        tokenExpr = tokenGlobal;
    } else {
        tokenExpr = tokenTmp;
        tokenFlag = 1;
    }
    return NO_ERR;
}

int getTokenExpr(token_t* tokenGlobal, token_t* tokenTmp, symtable_stack_t *symStack) {
    if (tokenFlag == 1){
        tokenExpr = tokenGlobal;
        tokenFlag = 2;
        return NO_ERR;
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


int checkExprSemantic(precedenceStackNode_t **top, int cnt, int *type, int rule) {

    fprintf(stderr, "rule : %d\n", rule);
    switch (rule) {
        case R_PLUS:
        case R_MINUS:
        case R_MUL:
        case R_DIV:
            fprintf(stderr,"i am okay first non terminal type is %d, second is %d\n",(*top)->next->next->type, (*top)->type);
            if( ((*top)->next->next->type >= ET_INT && (*top)->next->next->type <= ET_STRING) && ((*top)->type >= ET_INT && (*top)->type <= ET_STRING)  ){
                if ((*top)->next->next->type == (*top)->type) {
                    (*type) = (*top)->next->next->type;
                    return NO_ERR;
                } else if (rule != R_DIV) {
                    if ( ((*top)->next->next->type == ET_DOUBLE && (*top)->type == ET_INT) || ((*top)->next->next->type ==  ET_INT && (*top)->type == ET_DOUBLE) ) {
                        if((*top)->next->next->symbol == ES_NONTERL && (*top)->next->next->type == ET_INT) {
                            (*type) = ET_DOUBLE;
                            return NO_ERR;
                        } else if ((*top)->symbol == ES_NONTERL && (*top)->type == ET_INT) {
                            (*type) = ET_DOUBLE;
                            return NO_ERR;
                        }
                    } else if (rule == R_PLUS) {
                        if( (*top)->next->next->type == ET_STRING && (*top)->type == ET_STRING ){
                            (*type) = ET_STRING;
                            return NO_ERR;
                        }
                    }
                }
            }
            return printErrorAndReturn("Semantic error occured while reducing rule", SEM_ERR_TYPE_COMPAT);
            break;
        case R_EQ:
        case R_NEQ:
            if((*top)->next->next->type == (*top)->type) {
                return NO_ERR;
            } else if ( ((*top)->next->next->type == ET_DOUBLE && (*top)->type == ET_INT) || ((*top)->next->next->type ==  ET_INT && (*top)->type == ET_DOUBLE) ) {
                        if((*top)->next->next->symbol == ES_NONTERL && (*top)->next->next->type == ET_INT) {
                            return NO_ERR;
                        } else if ((*top)->symbol == ES_NONTERL && (*top)->type == ET_INT) {
                            return NO_ERR;
                        }
            }
            return printErrorAndReturn("Semantic error occured while reducing rule", SEM_ERR_TYPE_COMPAT);
            break;
        case R_L:
        case R_G:
        case R_LEQ:
        case R_GEQ:
            // if( ((*top)->next->next->type == (*top)->type) && ( ( (*top)->next->next->type < ET_INTN ) && ( (*top)->type < ET_INTN ) ) ) {
            //     return NO_ERR;
            // }
            fprintf(stderr, "fisrt type is %d symbol is %d, second is %d symbol is %d\n",(*top)->next->next->type, (*top)->next->next->symbol, (*top)->type, (*top)->symbol);
            if((*top)->next->next->type == (*top)->type) {
                return NO_ERR;
            } else if ( ((*top)->next->next->type == ET_DOUBLE && (*top)->type == ET_INT) || ((*top)->next->next->type ==  ET_INT && (*top)->type == ET_DOUBLE) ) {
                        if((*top)->next->next->symbol == ES_NONTERL && (*top)->next->next->type == ET_INT) {
                            return NO_ERR;
                        } else if ((*top)->symbol == ES_NONTERL && (*top)->type == ET_INT) {
                            return NO_ERR;
                        }
            }
            return printErrorAndReturn("Semantic error occured while reducing rule R_L", SEM_ERR_TYPE_COMPAT);
            break;
        case R_UNAR:
            if ((*top)->next->type >= T_INTN && (*top)->next->type <= T_STRINGN)
                (*type) = (*top)->next->type - 3;
            return NO_ERR;
        case R_ID:
            (*type) = (*top)->type;
            return NO_ERR;    
        case R_PAR:
            return NO_ERR;
        default:
            return SEM_ERR_TYPE_COMPAT;
    }
}

int reduceByRule(precedenceStackNode_t **top, int *cnt, int *type, symtable_stack_t *symStack){
    int rule = R_ERROR;
    int symbol = ES_NONTER;
    int error = SYNTAX_ERR;
    if ((*cnt) == 1) {
        if((*top)->symbol == ES_ID || (*top)->symbol == ES_LIT){
            int result = 0;
            if( (*top)->symbol == ES_ID) {
                fprintf(stderr, "%s\n",(*top)->content.str);
                result = checkInitialization(symStack, (*top)->content.str);  
            }
            symbol = (*top)->symbol+4; // ES_NONTER OR ES_NONTERMLITRAL
            if (result == 0) {
                rule = R_ID;
            } else {
                error = result;
                rule = R_ERROR;
            }
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
        if (((*top)->next->next->symbol == ES_OP_PAR) && ((*top)->next->symbol == ES_NONTER || (*top)->next->symbol == ES_NONTERL ) && ((*top)->symbol == ES_CL_PAR)) {
            rule =  R_PAR;
        } else if ( ((*top)->next->next->symbol == ES_NONTERL || (*top)->next->next->symbol == ES_NONTER) && ((*top)->symbol == ES_NONTER || (*top)->symbol == ES_NONTERL)) {
            if ((*top)->next->symbol < R_UNAR){
                rule = (*top)->next->symbol;
            } else { 
                rule = R_ERROR;
            }
        }
    }

    if (rule != R_ERROR ) {
        int result = 0;
        //char* content = '\0';
        result = checkExprSemantic(top, (*cnt), type, rule);
        if (result){
            return result;
        }
        for(int i = 0; i <= (*cnt); i++){
            prcStackPop(top);
        }
        prcStackPush(top, symbol, (*type), NULL);
        return NO_ERR;
    }

    return printErrorAndReturn("Syntax Error has occured in reduce by rule", error);
}

int expAnalyse (token_t* tokenGlobal, token_t* tokenTmp, int *type, symtable_stack_t *symStack) {

    token_t endToken;
    endToken.type = T_EOF; 
    precedenceStackNode_t* stack = NULL; 
    int stackItemsCounter = 0;
    prcStackPush(&stack, ES_END, ES_UNDEFINED, NULL);
    int result = 0;
    result = setTokenExpr(tokenGlobal, tokenTmp,symStack);
    if (result) {
        prcStackFree(&stack);
        return result;
    }

    do {
        precedenceStackNode_t* stackTerminal = prcStackGetTerminal(&stack);

        switch (precedenceTable[stackTerminal->symbol][getSymbolFromToken(tokenExpr)])
        {
        case '=':
            prcStackPush(&stack, getSymbolFromToken(tokenExpr), getTypeFromToken(tokenExpr, symStack), &(tokenExpr->content));
            if ((result = getTokenExpr(tokenGlobal, tokenTmp, symStack)) != 0)
                printErrorAndReturn("Lexical error has occured while expression analysing", result);
            break;
        case 'l':
            prcStackPushAfter(&stackTerminal, ES_CATCH, ES_UNDEFINED, NULL);
            prcStackPush(&stack, getSymbolFromToken(tokenExpr), getTypeFromToken(tokenExpr, symStack), &(tokenExpr->content));
            if ((result = getTokenExpr(tokenGlobal, tokenTmp, symStack)) != 0)
                printErrorAndReturn("Lexical error has occured while expression analysing", result);
            break;
        case 'g':
            if( ! (findCatch(&stack, &stackItemsCounter)) ) {
                result = reduceByRule(&stack, &stackItemsCounter, type, symStack);
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
    } while (((getSymbolFromToken(tokenExpr) != ES_END) || (prcStackGetTerminal(&stack)->symbol != ES_END)) && result == 0 );
    prcStackFree(&stack);
    return result;
}