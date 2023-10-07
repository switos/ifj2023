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
    ES_MUL,
    ES_PLUS,
    ES_ID,
    ES_END,
    ES_CATCH,
    ES_NONTER,
    ES_UNDEFINED,
} ES_TYPE;

typedef struct precedenceStackNode
{
    int symbol;
    int type;
    struct precedenceStackNode* next;
} precedenceStackNode_t;


int prcStackInit(precedenceStackNode_t **top, int symbol, int type) {
    (*top) = (precedenceStackNode_t*)malloc(sizeof(struct precedenceStackNode));
    fprintf(stderr, "out p : %p\n", top);
    if (top == NULL) {
        return printErrorAndReturn("Enternal error in prcStackInit", ERROR_INTERNAL); 
    }
    // printf("112");
    (*top)->symbol = symbol;
    (*top)->type = type;
    (*top)->next = NULL;
    return 0;
}

int prcStackFree(precedenceStackNode_t **top) {
    while ((*top) != NULL) {
        precedenceStackNode_t* tmp = (*top);
        (*top) = (*top)->next;
        free((*top));
    }
}

int prcStackPush(precedenceStackNode_t **top, int symbol, int type) {
    if ((*top) == NULL) {
        fprintf(stderr, "CHECKARINA\n");
        fprintf(stderr, "inp p : %p\n", top);
        prcStackInit(top, symbol, type);
        fprintf(stderr, "out out p : %p\n", top);
        if ((*top) == NULL) {
            fprintf(stderr, "CHECKARINA2\n");
        }
    } else {
        precedenceStackNode_t* new;
        prcStackInit(&new, symbol, type);
        new->next = (*top);
        (*top) = new;
    }
}

int prcStackGetTerminal(precedenceStackNode_t **top) {
    if((*top) != NULL) {
        return (*top)->symbol;
    } else {
        printf("ja pidor\n");
        exit(99);
    }
}