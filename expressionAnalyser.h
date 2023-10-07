#include "token.h"
#include "error.h"

/*
    |    | *  | +  | id | 
    | *  | >  | >  | <  | 
    | +  | <  | >  | >  |
    | id | <  | <  |    |
*/

char precedenceTable [3][3] = { 
    {'>', '>', '<'},
    {'<', '>', '>'},
    {'<', '<', ' '},
    };

typedef enum { 
    ES_ID,
    ES_PLUS,
    ES_MUL,
    ES_END,
    ES_CATCH,
    ES_NONTER,
    ES_UNDEFINED,
} ES_TYPE;

typedef struct precedenceStackNode
{
    int symbol;
    int type;
    string content;
    struct precedenceStackNode* next;
} precedenceStackNode_t;


int prcStackInnit(precedenceStackNode_t* top) {
    top = (struct precedenceStackNode*)malloc(sizeof(struct precedenceStackNode));
    if(top == NULL) {
        return printErrorAndReturn("Enternal error in prcStackInnit",ERROR_INTERNAL); 
    }
    top->symbol = ES_END;
    top->type = ES_UNDEFINED;
    str_init(&top->content);
}

int prcStackFree(precedenceStackNode_t* top) {
    while (top != NULL) {
        struct precedenceStackNode* tmp = top;
        top = top->next;
        str_free(&tmp->content);
        free(tmp);
    }
}