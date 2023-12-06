/*
IFJ projekt 2023
xtiemn00: Vsevolod Tiemnohorov
xpetro27: Denys Petrovsyi
xbatur00: Illia Baturov
xshish02: Sviatoslav Shishnev
*/
#include "scanner.c"
#include "symtable.c"

char precedenceTable [17][17] = { 
        //  +    -    *    /    ==  !=    <    >    <=   >=   ??   !    (    )    id  lit   $
   /*+*/  {'g', 'g', 'l', 'l', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'l', 'l', 'g', 'l', 'l', 'g'},
   /*-*/  {'g', 'g', 'l', 'l', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'l', 'l', 'g', 'l', 'l', 'g'},
   /***/  {'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'l', 'l', 'g', 'l', 'l', 'g'},
   /*/*/  {'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'l', 'l', 'g', 'l', 'l', 'g'},
  /*==*/  {'l', 'l', 'l', 'l', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'l', 'l', 'g', 'l', 'l', 'g'},
  /*!=*/  {'l', 'l', 'l', 'l', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'l', 'l', 'g', 'l', 'l', 'g'},
   /*<*/  {'l', 'l', 'l', 'l', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'l', 'l', 'g', 'l', 'l', 'g'},
   /*>*/  {'l', 'l', 'l', 'l', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'l', 'l', 'g', 'l', 'l', 'g'},
  /*<=*/  {'l', 'l', 'l', 'l', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'l', 'l', 'g', 'l', 'l', 'g'},
  /*>=*/  {'l', 'l', 'l', 'l', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'l', 'l', 'g', 'l', 'l', 'g'},
  /*??*/  {'l', 'l', 'l', 'l', 'l', 'l', 'l', 'l', 'l', 'l', 'l', 'l', 'l', 'g', 'l', 'l', 'g'},
   /*!*/  {'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'e', 'e', 'g', 'e', 'e', 'g'},
   /*(*/  {'l', 'l', 'l', 'l', 'l', 'l', 'l', 'l', 'l', 'l', 'l', 'l', 'l', '=', 'l', 'l', 'e'},
   /*)*/  {'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'l', 'e', 'g', 'e', 'e', 'g'},
  /*id*/  {'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'e', 'g', 'f', 'f', 'g'},
  /*lit*/ {'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'e', 'g', 'f', 'f', 'g'},
   /*$*/  {'l', 'l', 'l', 'l', 'l', 'l', 'l', 'l', 'l', 'l', 'l', 'l', 'l', 'e', 'l', 'l', 'e'},
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
    ES_LIT,
    ES_END,
    ES_CATCH,
    ES_NONTER,
    ES_NONTERL,
    ES_UNDEFINED,
} ES_SYMBOL;

typedef enum { 
    ET_INT = 18,
    ET_DOUBLE,
    ET_STRING,
    ET_INTN,
    ET_DOUBLEN,
    ET_STRINGN,
    ET_NIL = 24,
    ET_UNDEFINED = 95,
    ET_VOID,
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



int getTypeFromToken(token_t* token, symtable_stack_t *symStack) {
    if(token->type >= T_INT_LIT && token->type <= T_STRING_LIT ) {
        return token->type + 3;
    } else if (token->type == T_ID){
        htab_data_t* data;
        if ( (data = symtable_stack_search(symStack, token->content.str)) == NULL) {
            return ET_UNDEFINED;
        }
        return (data->type);
    } else if (token->type == T_NIL) {
        return ET_NIL;
    } else {
        return ET_UNDEFINED;
    }
}

int getSymbolFromToken(token_t* token) {
    if(token->type < T_ID) {
        return token->type;
    } else if (token->type == T_ID) {
        return ES_ID;
    } else if ((token->type <= T_STRING_LIT && token->type >= T_INT_LIT) || token->type == T_NIL) {
        return ES_LIT;
    } else {
        return ES_END;
    }
}

void chooseContent(int es, string* target, string* source) {
    if ( es == ES_ID || es == ES_LIT) {
        str_copy_string(target, source);
    }

    return;
}

typedef struct precedenceStackNode
{
    int symbol;
    int type;
    string content;
    struct precedenceStackNode* next;
} precedenceStackNode_t;



int prcStackInit(precedenceStackNode_t** top, int symbol, int type, string *content) {
    (*top) = (precedenceStackNode_t*)malloc(sizeof(struct precedenceStackNode));
    if ((*top) == NULL) {
        return printErrorAndReturn("Enternal error in prcStackInit", ERROR_INTERNAL); 
    }
    str_init(&((*top)->content));
    if ( content != NULL ) {
        fprintf(stderr, "source is %s\n", content->str);
        chooseContent(symbol, &((*top)->content), content);
    }
    (*top)->symbol = symbol;
    (*top)->type = type;
    (*top)->next = NULL;
    return 0;
}


int prcStackPush(precedenceStackNode_t** top, int symbol, int type, string *content) {

    if ((*top) == NULL) {
        prcStackInit(top, symbol, type, content);
    } else {
        precedenceStackNode_t* new;
        prcStackInit(&new, symbol, type, content);
        new->next = (*top);
        (*top) = new;

    }
    return 0;
}

int prcStackPushAfter(precedenceStackNode_t** node, int symbol, int type, string *content) {
    if ((*node) == NULL) {
        return 1;
    } else {
        precedenceStackNode_t *new;    
        prcStackInit(&new, (*node)->symbol, (*node)->type, &((*node)->content));
        new->next = (*node)->next;
        (*node)->next = new;
        (*node)->symbol = symbol;
        (*node)->type = type;
        if(content != NULL) {
            str_copy_string(&((*node)->content), content);
        }

        return 0;
    }
}

int prcStackPop(precedenceStackNode_t** top){
    if((*top) != NULL){
        str_free(&((*top)->content));
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

int prcStackFree(precedenceStackNode_t** top) {
    while (prcStackPop(top) == 0) {

    }
    return 0;
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