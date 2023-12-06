/*
IFJ projekt 2023
xtiemn00: Vsevolod Tiemnohorov
xpetro27: Denys Petrovsyi
xbatur00: Illia Baturov
xshish02: Sviatoslav Shishnev
*/
#include "expressionAnalyser.h"


int TypeMap(int type, int exp) {
    switch (type) {
        case ET_DOUBLE:
            if(exp == ET_DOUBLE || exp == ET_INT)
                return 0;
            return 1; 
        case ET_INT:
            if (exp == ET_INT)
                return 0;
            return 1;
        case ET_STRING:
            if(exp != ET_STRING)
                return 1;
            return 0;
        case ET_DOUBLEN:
            if(exp == ET_DOUBLE || exp == ET_INT || exp == ET_DOUBLEN || exp == ET_INTN || exp == ET_NIL)
                return 0;
            return 1;
        case ET_INTN:
            if (exp == ET_INT || exp == ET_NIL || exp == ET_INTN )
                return 0;
            return 1;
        case ET_STRINGN:
            if(exp == ET_STRINGN || exp == ET_STRING || exp == ET_NIL)//&& exp != ET_NIL
                return 0;
            return 1;
        case ET_VOID:
            return 1;
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
    if (data->argumentsInArray != 0 || data->constant == true) {
        return printErrorAndReturn("Wrong number of arguments", SEM_ERR_WRONG_PARAM);
    }
    return NO_ERR;
}

int checkArgument(symtable_stack_t *symStack, char *name, char *argName, int type, int number) {
    symtable_t *table = symtable_get_global(symStack);

    if (table == NULL) {
        return printErrorAndReturn("Enternal error in Check argument", ERROR_INTERNAL);
    }

    htab_data_t * data = symtable_search(table, name); 

    if (data->isVar == true) {
        return printErrorAndReturn("Variable called instead of function not defined", SEM_ERR_OTHER);
    }
    

    if(data->constant == true) 
        return NO_ERR;

    if (data->argumentAmount == 0 || data->argumentsInArray < number){
        return printErrorAndReturn("Wrong number of arguments", SEM_ERR_WRONG_PARAM);
    }
    data_param_t *argument = symtable_get_argument(table, name, number);
    if(argument == NULL ) {
        return printErrorAndReturn("NULL arguments in checkArgument, something gone wrong", SEM_ERR_WRONG_PARAM);
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
        return printErrorAndReturn("Semantic error occured in funAddArguments", SEM_ERR_UNDEFINED_FUNCTION);
    if (data->initialized == false) { 
        if ( !(symtable_add_arguments(data, argName, argId, type)) ) {
            fprintf(stderr, "Add arguments do not work\n");
        }
    }
    return NO_ERR;
}

int addArg2BuildInFun(symtable_stack_t *symStack, char *name, char *argName, char *argId, int type) {
    htab_data_t *data = symtable_stack_search(symStack, name);
    if (data == NULL)
        return printErrorAndReturn("NUUL in add argument", SEM_ERR_UNDEFINED_FUNCTION);
    if ( !(symtable_add_arguments(data, argName, argId, type)) ) {
        fprintf(stderr, "Add arguments do not work\n");
    }
    return 0;
}


int funDefiner(symtable_stack_t *symStack, int type, char *name) {
    htab_data_t *data = symtable_search (symtable_get_global(symStack),name);
    if (data == NULL){
        symtable_insert_data(&(symStack->top->table), name, type, name, false, false, false);
        return NO_ERR;
    } else {
        if (data->initialized == false ) {
            data->initialized = true;
            return NO_ERR;
        }
    }
    return printErrorAndReturn("Semantic error occured in funDefiner", SEM_ERR_OTHER);
}

int varDefiner(symtable_stack_t *symStack, int type, char* name, bool inicialized, bool constant ) {
    symtable_insert_data(&(symStack->top->table), name, type, name, inicialized, constant, true);
    return 0;
}

int set_nil(symtable_stack_t *symStack, char* name) {
    htab_data_t *data = symtable_stack_search(symStack, name);
    if (data == NULL)
        return printErrorAndReturn("Undefined variable or function in Definition check", SEM_ERR_UNDEFINED_VAR);
    data->nil = true;
    data->initialized = true;
    return 0;    
}

int checkDefinition(symtable_stack_t *symStack, char* name) {
    htab_data_t *data = symtable_stack_search(symStack, name);
    if (data == NULL)
        return printErrorAndReturn("Undefined variable or function in Definition check", SEM_ERR_UNDEFINED_VAR);
    return 0;
}

int checkInitialization(symtable_stack_t *symStack, char* name ) {
    fprintf(stderr, "name on defenition check is %s\n", name);
    htab_data_t *data = symtable_stack_search(symStack, name);
    if (data == NULL)
        return printErrorAndReturn("Undefined variable in InitCheck, on definition check", SEM_ERR_UNDEFINED_VAR);
    if (data->initialized != true && data->isVar == true) 
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
        if(exp >= ET_INT && exp <= ET_STRINGN){
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

int returnSemanticCheck(symtable_stack_t *symStack, char* name, int exp){
    htab_data_t *data = symtable_search (symtable_get_global(symStack),name);
    if ((data->type == ET_VOID && exp != ET_VOID) || (data->type != ET_VOID && exp == ET_VOID) ) {
        return printErrorAndReturn("Semantic error function return type compatibility", SEM_ERR_WRONG_RET);
    }
    if (data->type != exp) {
        return printErrorAndReturn("Semantic error function return type compatibility", SEM_ERR_WRONG_PARAM);
    }
    return 0;
}

int returnExistingCheck(bool functionBodyFlag, bool *returnFlag) {
    if( functionBodyFlag ) {
        if ( !returnFlag )
            return printErrorAndReturn("Return error has occured in localParse", SYNTAX_ERR);
        returnFlag = false;
    }
    return NO_ERR;
}

int argAmountCheck(symtable_stack_t *symStack, char* name, int amount) {
    
    htab_data_t *data = symtable_search (symtable_get_global(symStack),name);
    if (((data->argumentsInArray ) == amount + 1 )|| data->constant == true ) 
        return NO_ERR;
    return printErrorAndReturn("Wrong number of arguments", SEM_ERR_WRONG_PARAM);
}

int pushArguments(symtable_stack_t *symStack, char *name) {
    symtable_t *table = symtable_get_global(symStack);

    if (table == NULL) {
        return printErrorAndReturn("Enternal error in Check argument", ERROR_INTERNAL);
    }

    htab_data_t * data = symtable_search(table, name); 

    int cnt = 0;

     data_param_t *argument = NULL;

    while(cnt < data->argumentsInArray){
        argument = symtable_get_argument(table, name, cnt);
        if(argument == NULL ) {
            fprintf(stderr, "NULL arguments in pushArguments, something gone wrong");
        }
        varDefiner(symStack, argument->type, argument->identifier, true, true);
        cnt++;
    }

    return 0;
}


void buildInFunctionDefenition(symtable_stack_t *symStack) {
    symtable_insert_data(&(symStack->top->table), "readString", ET_STRINGN, "readString", true, false, false); // func readString() -> String?
    symtable_insert_data(&(symStack->top->table), "readInt", ET_INTN, "readInt", true, false, false); // func readInt() -> Int?
    symtable_insert_data(&(symStack->top->table), "readDouble", ET_DOUBLEN, "readDouble", true, false, false); // func readDouble() -> Double?
    symtable_insert_data(&(symStack->top->table), "write", ET_VOID, "write", true, true, false); // func write ( term1 , term2 , …, term𝑛 )
    symtable_insert_data(&(symStack->top->table), "Int2Double", ET_DOUBLE, "Int2Double", true, false, false); //func Int2Double(_ term ∶ Int) -> Double 
    addArg2BuildInFun(symStack, "Int2Double", "_", "term", ET_INT);
    symtable_insert_data(&(symStack->top->table), "Double2Int", ET_INT, "Double2Int", true, false, false); //func Double2Int(_ term ∶ Double) -> Int
    addArg2BuildInFun(symStack, "Double2Int", "_", "term", ET_DOUBLE);
    symtable_insert_data(&(symStack->top->table), "length", ET_INT, "length", true, false, false); //func length(_ 𝑠 : String) -> Int
    addArg2BuildInFun(symStack, "length", "_", "s", ET_STRING);
    symtable_insert_data(&(symStack->top->table), "substring", ET_STRINGN, "substring", true, false, false); //func substring(of 𝑠 : String, startingAt 𝑖 : Int, endingBefore 𝑗 : Int) -> String?
    addArg2BuildInFun(symStack, "substring", "of", "s", ET_STRING);
    addArg2BuildInFun(symStack, "substring", "startingAt", "i", ET_INT);
    addArg2BuildInFun(symStack, "substring", "endingBefore", "j", ET_INT);
    symtable_insert_data(&(symStack->top->table), "ord", ET_INT, "ord", true, false, false); //func ord(_ 𝑐 : String) -> Int
    addArg2BuildInFun(symStack, "ord", "_", "c", ET_STRING);
    symtable_insert_data(&(symStack->top->table), "chr", ET_STRING, "chr", true, false, false); //func chr(_ 𝑖 : Int) -> String
    addArg2BuildInFun(symStack, "chr", "_", "i", ET_INT);

}
