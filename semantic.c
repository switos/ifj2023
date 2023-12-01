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
        if(type != exp && type-3 != exp) { //&& exp != ET_NIL
            return 1;
        }
        return 0;
    default:
        fprintf(stderr, "DEFAULT IN TYPEMAP FUNCTION, SOMETHING REALLY BAD HAPPEND\n");
        return 1;
    }
}




int zeroArgsCheck(symtable_stack_t *symStack, char *name) {
    htab_data_t *data = symtable_stack_search(symStack, name);
    if (data == NULL) {
        return printErrorAndReturn("Enternal error in Check argument", ERROR_INTERNAL);
    }
    if (data->argumentsInArray != 0) {
        return printErrorAndReturn("Wrong number of arguments", SEM_ERR_WRONG_PARAM);
    }
    return NO_ERR;
}

int checkArgument(symtable_stack_t *symStack, char *name, char *argName, int type, int number) {
    symtable_t *table = symtable_get_global(symStack);
    if (table == NULL) {
        return printErrorAndReturn("Enternal error in Check argument", ERROR_INTERNAL);
    }
    
    data_param_t *argument = symtable_get_argument(table, name, number);
    if(argument == NULL ) {
        return printErrorAndReturn("Wrong number of arguments", SEM_ERR_WRONG_PARAM);
    }

    if (strcmp(argument->name, argName)) {
        return printErrorAndReturn("Wrong name of argument", SEM_ERR_OTHER);
    }

    if (TypeMap(argument->type, type)) {
        return printErrorAndReturn("Wrong type of argument", SEM_ERR_WRONG_PARAM);
    }


    return NO_ERR;
}

int setType(symtable_stack_t *symStack, char *name, int type) {
    htab_data_t *data = symtable_stack_search(symStack, name);
    if (data == NULL)
        return printErrorAndReturn("Semantic error occured in SetType", SEM_ERR_UNDEFINED_FUNCTION);
    data->type = type;
    return NO_ERR;
}

int funAddArgument(symtable_stack_t *symStack, char *name, char *argName, char *argId, int type) {
    htab_data_t *data = symtable_stack_search(symStack, name);
    if (data == NULL)
        return printErrorAndReturn("Semantic error occured in SetType", SEM_ERR_UNDEFINED_FUNCTION);
    if (data->initialized == false) { 
        if ( !(symtable_add_arguments(data, argName, argId, type)) ) {
            fprintf(stderr, "Add arguments do not work\n");
        }
    }
    return NO_ERR;
}

int funDefiner(symtable_stack_t *symStack, int type, char *name) {
    htab_data_t *data = symtable_stack_search(symStack, name);
    if (data == NULL){
        symtable_insert_data(symStack->top->table, name, type, name, false, false, 10);
        return NO_ERR;
    } else {
        if (data->initialized == false ) {
            data->initialized = true;
            return NO_ERR;
        }
    }
    return printErrorAndReturn("Semantic error occured in funDefiner", SEM_ERR_UNDEFINED_FUNCTION);
}

int varDefiner(symtable_stack_t *symStack, int type, char* name, bool inicialized, bool modify ) {
    symtable_insert_data(symStack->top->table, name, type, name, inicialized, modify, -1);
    return 0;
}

int checkDefinition(symtable_stack_t *symStack, char* name) {
    htab_data_t *data = symtable_stack_search(symStack, name);
    if (data == NULL)
        return printErrorAndReturn("Undefined variable or function in Definition check", SEM_ERR_UNDEFINED_VAR);
    return 0;
}

int checkInitialization(symtable_stack_t *symStack, char* name ) {
    htab_data_t *data = symtable_stack_search(symStack, name);
    if (data == NULL)
        return printErrorAndReturn("Undefined variable in InitCheck, on defenition check", SEM_ERR_UNDEFINED_VAR);
    if (data->initialized != true) 
        return printErrorAndReturn("Uninitialized variable in InitCheck", SEM_ERR_UNDEFINED_VAR);
    return 0;
}

int mutableCheck(symtable_stack_t *symStack, char* name) {
    htab_data_t *data = symtable_stack_search(symStack, name);
    if (data->constant == true)
        return 1;
    else 
        return 0;
}

int idCheckType(symtable_stack_t *symStack, char* name, int type) {
    htab_data_t *data = symtable_stack_search(symStack, name);
    if (TypeMap(data->type, type) == 0) { 
        data->initialized = true;
        return NO_ERR;
    }
    return printErrorAndReturn("Sematic error has occured in idCheckType, type incompatible", SEM_ERR_TYPE_COMPAT);
}

int idCheck(symtable_stack_t *symStack, char* name) {
    int result = 0;
    if ((result = checkDefinition(symStack, name))) 
        return result;
    if (mutableCheck(symStack, name))
        return printErrorAndReturn("Sematic error has occured in idCheck, unmutable variable", SEM_ERR_OTHER);
    return NO_ERR;
}

int VarDefAssignSemanticCheck(int *type, int exp){

    if ((*type) == ET_UNDEFINED) {
        if(exp >= ET_INT){
            (*type) = exp;
            return NO_ERR;
        }
        return printErrorAndReturn("Sematic error has occured in varDefItem", SEM_ERR_TYPE_GETTING);
    } else {
        if(TypeMap((*type), exp)){ 
            return printErrorAndReturn("Sematic error has occured in varDefItem, type incompatible", SEM_ERR_TYPE_COMPAT);
        } 
        return NO_ERR;
    }

}

void buildInFunctionDefenition(){
    
}
