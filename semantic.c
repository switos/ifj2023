#include "expressionAnalyser.h"


int TypeMap(int type, int exp) {
    switch (type)
    {
    case ET_DOUBLE:
    case ET_INT:
    case ET_STRING:
        if(type != exp){
            return 1;
        }
        return 0;
    case ET_DOUBLEN:
    case ET_INTN:
    case ET_STRINGN:
        if(type != exp && type-3 != exp && exp != ET_NIL) {
            return 1;
        }
        return 0;
    default:
        fprintf(stderr, "DEFAULT IN TYPEMAP FUNCTION, SOMETHING REALLY BAD HAPPEND\n");
        return 1;
    }
}

int VarDefAssignSemanticCheck(int *type, int exp){

    if ((*type) == ET_UNDEFINED) {
        if(exp > ET_NIL){
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

