#include "scanner.c"
#include "symtable.c"

char precedenceTable [16][16] = { 
        //  +    -    *    /    ==  !=    <    >    <=   >=   ??   !    (    )    id   $
   /*+*/  {'g', 'g', 'l', 'l', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'l', 'l', 'g', 'l', 'g'},
   /*-*/  {'g', 'g', 'l', 'l', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'l', 'l', 'g', 'l', 'g'},
   /***/  {'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'l', 'l', 'g', 'l', 'g'},
   /*/*/  {'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'l', 'l', 'g', 'l', 'g'},
  /*==*/  {'l', 'l', 'l', 'l', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'l', 'l', 'g', 'l', 'g'},
  /*!=*/  {'l', 'l', 'l', 'l', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'l', 'l', 'g', 'l', 'g'},
   /*<*/  {'l', 'l', 'l', 'l', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'l', 'l', 'g', 'l', 'g'},
   /*>*/  {'l', 'l', 'l', 'l', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'l', 'l', 'g', 'l', 'g'},
  /*<=*/  {'l', 'l', 'l', 'l', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'l', 'l', 'g', 'l', 'g'},
  /*>=*/  {'l', 'l', 'l', 'l', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'l', 'l', 'g', 'l', 'g'},
  /*??*/  {'l', 'l', 'l', 'l', 'l', 'l', 'l', 'l', 'l', 'l', 'l', 'l', 'l', 'g', 'l', 'g'},
   /*!*/  {'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'e', 'e', 'g', 'e', 'g'},
   /*(*/  {'l', 'l', 'l', 'l', 'l', 'l', 'l', 'l', 'l', 'l', 'l', 'e', 'l', '=', 'l', 'e'},
   /*)*/  {'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'l', 'e', 'g', 'e', 'g'},
  /*id*/  {'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'e', 'g', 'f', 'g'},
   /*$*/  {'l', 'l', 'l', 'l', 'l', 'l', 'l', 'l', 'l', 'l', 'l', 'l', 'l', 'e', 'l', 'e'},
};

typedef enum { 
    ES_PLUS,
    ES_MINUS,
    ES_MUL,
    ES_DIV,
    ES_EQ,
    ES_NEQ,
    ES_LESS,
    ES_GR,
    ES_LESSEQ,
    ES_GREQ,
    ES_NIL_COAL, // ??
    ES_EX, // !
    ES_OP_PAR, // ( 
    ES_CL_PAR, // )
    ES_ID = 14,
    ES_END,
    ES_CATCH,
    ES_NONTER,
    ES_UNDEFINED,
} ES_SYMBOL;

typedef enum { 
    ET_NIL = 18,
    ET_DOUBLE,
    ET_INT,
    ET_STRING,
    ET_DOUBLEN,
    ET_INTN,
    ET_STRINGN,
    ET_UNDEFINED,
} ET_TYPE;

typedef enum {
    R_PLUS,
    R_MINUS,
    R_MUL,
    R_DIV,
    R_EQ,
    R_NEQ,
    R_L,
    R_G,
    R_LEQ,
    R_GEQ,
    R_NILCON,
    R_UNAR,
    R_PAR,
    R_ID,
    R_ERROR
} R_RULE;



int getSymbolFromToken(token_t* token) {
    if(token->type < T_ID) {
        return token->type;
    } else if (token->type <= T_NIL) {
        return ES_ID;
    } else {
        return ES_END;
    }
}

typedef struct precedenceStackNode
{
    int symbol;
    int type;
    int lit;
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
        fprintf(stderr,"i am free\n");
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
    return 0;
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
        return 0;
    }
}

int prcStackPop(precedenceStackNode_t** top){
    if((*top) != NULL){
        fprintf(stderr,"let's go\n");
        precedenceStackNode_t* tmp = (*top);
        (*top) = (*top)->next;
        free(tmp);
        tmp = NULL;
        return 0;
    } else {
        fprintf(stderr,"stack is empty\n");
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