#include "expressionAnalyser.h"


int TypeMap(int type, int exp) {
    switch (type-4)
    {
    case ET_DOUBLE:
    case ET_INT:
    case ET_STRING:
        if(type != exp+4){
            return 1;
        }
        return 0;
    case ET_DOUBLEN:
    case ET_INTN:
    case ET_STRINGN:
        if(type != exp+4 && type-3 != exp+4) { //&& exp != ET_NIL
            return 1;
        }
        return 0;
    default:
        fprintf(stderr, "DEFAULT IN TYPEMAP FUNCTION, SOMETHING REALLY BAD HAPPEND\n");
        return 1;
    }
}

int varDefiner(symtable_stack_t *symStack, int type, char* name, bool inicialized, bool modify ) {
    symtable_insert_data(symStack->top->table, name, type, name, inicialized, modify, -1);
    return 0;
}


int VarDefAssignSemanticCheck(int *type, int exp){

    if ((*type) == ET_UNDEFINED) {
        if(exp >= ET_INT){
            (*type) = exp;
            return NO_ERR;
        }
        return printErrorAndReturn("Sematic error has occured in varDefItem", SEM_ERR_OTHER);
    } else {
        if(TypeMap((*type), exp)){ 
            return printErrorAndReturn("Sematic error has occured in varDefItem, type incompatible", SEM_ERR_TYPE_COMPAT);
        } 
        return NO_ERR;
    }

}


