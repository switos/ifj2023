#include "scanner.c"

char precedenceTable [16][16] = { 
        //  +    -    *    /    ==  !=    <   >     <=   >=   ??   !    (    )    id   $
   /*+*/  {'g', 'g', 'l', 'l', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'l', 'l', 'g', 'l', 'g'},
   /*-*/  {'g', 'g', 'l', 'l', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'l', 'l', 'g', 'l', 'g'},
   /***/  {'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'l', 'l', 'g', 'l', 'g'},
   /*/*/  {'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'l', 'l', 'g', 'l', 'g'},
  /*==*/  {'l', 'l', 'l', 'l', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'l', 'l', 'g', 'l', 'g'},
  /*!=*/  {'l', 'l', 'l', 'l', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'l', 'l', 'g', 'l', 'g'},
   /*>*/  {'l', 'l', 'l', 'l', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'l', 'l', 'g', 'l', 'g'},
   /*<*/  {'l', 'l', 'l', 'l', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'l', 'l', 'g', 'l', 'g'},
  /*<=*/  {'l', 'l', 'l', 'l', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'l', 'l', 'g', 'l', 'g'},
  /*>=*/  {'l', 'l', 'l', 'l', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'l', 'l', 'g', 'l', 'g'},
  /*??*/  {'l', 'l', 'l', 'l', 'l', 'l', 'l', 'l', 'l', 'l', 'l', 'l', 'l', 'g', 'l', 'g'},
   /*!*/  {'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'e', 'e', 'g', 'e', 'g'},
   /*(*/  {'l', 'l', 'l', 'l', 'l', 'l', 'l', 'l', 'l', 'l', 'l', 'e', 'l', '=', 'l', 'e'},
   /*)*/  {'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'l', 'e', 'g', 'e', 'g'},
  /*id*/  {'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'e', 'e', 'g', 'e', 'g'},
   /*$*/  {'l', 'l', 'l', 'l', 'l', 'l', 'l', 'l', 'l', 'l', 'l', 'l', 'l', 'e', 'l', 'e'},
};

typedef enum { 
    ES_PLUS,
    ES_MINUS,
    ES_MUL,
    ES_DIV,
    ES_EX,
    ES_ID = 14,
    ES_END,
    ES_CATCH,
    ES_NONTER,
    ES_UNDEFINED,
} ES_SYMBOL;

typedef enum {
    R_ID,
    R_PLUS,
    R_MINUS,
    R_MUL,

    R_DIV,
    R_UNAR,
    R_ERROR
} R_RULE;

int getSymbolFromToken(token_t* token) {
    if(token->type < T_ID) {
        return token->type;
    } else if (token->type < T_EOF) {
        return ES_ID;
    } else {
        return ES_END;
    }
}

typedef struct precedenceStackNode
{
    int symbol;
    int type;
    struct precedenceStackNode* next;
} precedenceStackNode_t;



int prcStackInit(precedenceStackNode_t** top, int symbol, int type) {
    (*top) = (precedenceStackNode_t*)malloc(sizeof(struct precedenceStackNode));
    if ((*top) == NULL) {
        return printErrorAndReturn("Enternal error in prcStackInit", ERROR_INTERNAL); 
    }
    (*top)->symbol = symbol;
    (*top)->type = type;
    (*top)->next = NULL;
    return 0;
}

int prcStackFree(precedenceStackNode_t** top) {
    while ((*top) != NULL) {
        precedenceStackNode_t* tmp = (*top);
        (*top) = (*top)->next;
        free(tmp);
        printf("i am free\n");
    }
    return 0;
}

int prcStackPush(precedenceStackNode_t** top, int symbol, int type) {
    if ((*top) == NULL) {
        prcStackInit(top, symbol, type);
    } else {
        precedenceStackNode_t* new;
        prcStackInit(&new, symbol, type);
        new->next = (*top);
        (*top) = new;
    }
}
int prcStackPushAfter(precedenceStackNode_t** node, int symbol, int type) {
    if ((*node) == NULL) {
        return 1;
    } else {
        precedenceStackNode_t* new;
        prcStackInit(&new, (*node)->symbol, (*node)->type);
        new->next = (*node)->next;
        (*node)->next = new;
        (*node)->symbol = symbol;
        (*node)->type = type;
    }
}

int prcStackPop(precedenceStackNode_t** top){
    if((*top) != NULL){
        printf("let's go\n");
        precedenceStackNode_t* tmp = (*top);
        (*top) = (*top)->next;
        free(tmp);
        tmp == NULL;
        return 0;
    } else {
        printf("stack is empty\n");
        return 1;
    }
}

precedenceStackNode_t* prcStackGetTerminal(precedenceStackNode_t** top) {
    precedenceStackNode_t* current = (*top);
    while (current != NULL)
    {
        if(current->symbol < ES_CATCH) 
            return current;
        current = current->next;
    }
    return NULL;   
}