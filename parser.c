/*
IFJ projekt 2023
xtiemn00: Vsevolod Tiemnohorov
xpetro27: Denys Petrovsyi
xbatur00: Illia Baturov
xshish02: Sviatoslav Shishnev
*/

#include "parser.h"
token_t token;
precedenceStackNode_t* prcStack;
symtable_stack_t symStack;
DLList list;
string varName;
string funName;
string funBodyName;
string argName;
string argId;
int argumentNumber = 0;
bool firstAnalyseFlag = true;
bool functionBodyFlag = false;

void freeAll() {
    symtable_stack_free(&symStack);
    str_free(&token.content);
    str_free(&funName);
    str_free(&funBodyName);
    str_free(&varName);
    str_free(&argName);
    str_free(&argId);
    // DLL_Dispose(&list);
}

void initAll() {
    str_init(&token.content);
    str_init(&varName);
    str_init(&funName);
    str_init(&argName);
    str_init(&argId);
    symtable_stack_init(&symStack);
    // DLL_Init(&list);
}

int newLineCheck() {
    if (token.newLineFlag == false)
        return 1;
    return 0;
}

int litCheck(){
    if (token.type == T_FLOAT_LIT || token.type == T_STRING_LIT || token.type == T_INT_LIT || token.type == T_NIL) 
        return 1;
    return 0;

} 


int typeCheck(){
    if (token.type >= T_INT && token.type <= T_STRINGN) {
        return 1;
    } else {
        return 0;
    }
}

void exitAndFree(int etype) {
    freeAll();
    fprintf(stderr,"exit code is %d\n", etype);
    exit(etype);
}

void getTokenWrapped() {
    if (getToken(&token)) {
        exitAndFree(LEX_ERR); 
    }
}

int expression(int *expType) {
    if (litCheck() || token.type == T_OP_PAR) {
        return expAnalyse(&token, NULL, expType, &symStack);
    } else if (token.type == T_ID) {
        token_t tmpToken;
        str_init(&(tmpToken.content));
        str_copy_string(&(tmpToken.content), &(token.content));
        tmpToken.type = token.type;
        getTokenWrapped();
        if( token.type == T_OP_PAR){
            str_copy_string(&funName, &(tmpToken.content));
            str_free(&(tmpToken.content));
            return funCall(expType);
        } else {
            int result = expAnalyse(&token, &tmpToken, expType, &symStack);
            str_free(&(tmpToken.content));
            return result;
        }
    }
    return printErrorAndReturn("Syntaxe error has occured in expression or function rule", SYNTAX_ERR);
}

int id() {
    int result = 0;
    int expType;
    str_copy_string(&varName, &(token.content)); //save id
    getTokenWrapped();
    if (token.type == T_ASSING) {
        if ((result = idCheck(&symStack, varName.str))) 
            return result;
        getTokenWrapped();
        result = expression(&expType);
        if (result == 0)
            result = idCheckType(&symStack, varName.str, expType);
        return result;
    } else if (token.type == T_OP_PAR) {
        str_copy_string(&funName, &varName);
        return funCall(&expType);
    }
    return printErrorAndReturn("Syntaxe error has occured in id", SYNTAX_ERR);
}

int varDefItem(bool constant, int type) {
    int typeTmp = type;
    int result;
    if(token.type == T_ASSING) {
            getTokenWrapped();
            int expType = ET_UNDEFINED;
            if ((result = expression(&expType)))
                return result;
            fprintf(stderr,"TOKEN TYPE %d %d name is %s\n", type, expType, varName.str);
            if((result = VarDefAssignSemanticCheck(&typeTmp, expType)))
                return result;
            varDefiner(&symStack, typeTmp, varName.str, true, constant);
            if (symtable_stack_search(&symStack, varName.str) == NULL) {
                fprintf(stderr,"Var definer dont work in varDefItem\n");
            }
            return NO_ERR;
    } else {
        varDefiner(&symStack, typeTmp, varName.str, false, constant);
        if( typeTmp <= T_STRINGN && typeTmp >= T_INTN )
            set_nil(&symStack, varName.str);
        return NO_ERR;
    }
    return printErrorAndReturn("Syntax error has occured in varDefItem", SYNTAX_ERR);
} 

int varDefList(bool constant) {
    if(token.type == T_COLON) {
        getTokenWrapped();
        if(typeCheck()) {
            int type = token.type;
            fprintf(stderr,"TOKEN TYPE %d \n",token.type);
            getTokenWrapped();
            return varDefItem(constant, type);
        }
    } else if(token.type == T_ASSING) {
        return varDefItem(constant, ET_UNDEFINED);
    }
    return printErrorAndReturn("Syntaxe error in VarDefList", SYNTAX_ERR);
}

int varDef() {
    bool constant;
    if(token.type == T_LET) {
        constant = true;
    } else {
        constant = false;
    }
    getTokenWrapped();
    if(token.type ==  T_ID) {
        str_copy_string(&varName, &(token.content));
        if(symtable_search(symStack.top->table, varName.str) != NULL)
            return printErrorAndReturn("Sematic error in VarDef, redefinition", SEM_ERR_UNDEFINED_FUNCTION);
        getTokenWrapped();
        return varDefList(constant);
    }
    return printErrorAndReturn("Syntaxe error in VarDef", SYNTAX_ERR);
}

int funDefType() {
    functionBodyFlag = true;
    if (token.type == T_OP_BRACE) {
        getTokenWrapped();
        setType(&symStack, funName.str, ET_VOID);
        if (firstAnalyseFlag) 
            return NO_ERR;  
        tFlagS(&token);
        symtable_stack_push(&symStack);
        pushArguments(&symStack, funName.str);
        return functionParse();
    } else if (token.type == T_ARROW ){
        getTokenWrapped();
        if (typeCheck()) {
            setType(&symStack, funName.str, token.type);
            getTokenWrapped();
            if (token.type == T_OP_BRACE) {
                getTokenWrapped();
                tFlagS(&token);
                if (firstAnalyseFlag) 
                    return NO_ERR;  
                symtable_stack_push(&symStack);
                pushArguments(&symStack, funName.str);
                return functionParse();
            }
        }
    }
    return printErrorAndReturn("Syntax error has occured in funDefType", SYNTAX_ERR);
}

int funDefItem() {
    int result = 0; 
    if (token.type == T_COMMA) {
        getTokenWrapped();
        if(token.type == T_ID || token.type == T_UNDER) {
            str_copy_string(&argName, &token.content);
            getTokenWrapped();
            if(token.type == T_ID || token.type == T_UNDER) {
                str_copy_string(&argId, &token.content);
                getTokenWrapped();
                if (token.type == T_COLON) {
                    getTokenWrapped();
                    if (typeCheck()) {
                        result = funAddArgument(&symStack, funName.str, argName.str, argId.str, token.type);
                        if (result) {
                            return result;
                        }
                        getTokenWrapped();
                        return funDefItem();
                    }
                }
            }
        }
    } else if (token.type == T_CL_PAR) {
        getTokenWrapped();
        return funDefType();
    }
    return printErrorAndReturn("Syntaxe error has occured in funDefItem", SYNTAX_ERR);
}

int funDefPlist() {
    if (token.type == T_CL_PAR){
        getTokenWrapped();
        return funDefType();
    } else if(token.type == T_ID || token.type == T_UNDER) {
        str_copy_string(&argName, &token.content);
        getTokenWrapped();
        if(token.type == T_ID || token.type == T_UNDER) {
            str_copy_string(&argId, &token.content);
            getTokenWrapped();
            if (token.type == T_COLON) {
                getTokenWrapped();
                if (typeCheck()) {
                    funAddArgument(&symStack, funName.str, argName.str, argId.str, token.type);
                    getTokenWrapped();
                    return funDefItem();
                }
            }
        }
    }
    return printErrorAndReturn("Syntaxe error has occured in funDefPlist", SYNTAX_ERR);
}

int funDef() {
    int result = 0;
    getTokenWrapped();
    if (token.type == T_ID) {   
        str_copy_string(&funName, &(token.content)); //save id
        getTokenWrapped();
        if(token.type == T_OP_PAR) {  
            getTokenWrapped();
            result = funDefiner(&symStack, ET_UNDEFINED, funName.str);
            return result ? result : funDefPlist();
        }
    }
    return printErrorAndReturn("Syntax error has occured in funDef", SYNTAX_ERR);
}
 

// Function call is bellow

int parItem(){
    int result = 0;
    if (token.type == T_ID) {
        str_copy_string(&argName, &(token.content));
        getTokenWrapped();
        return parListId();
    } else if (litCheck()) {
        result = checkArgument(&symStack, funName.str, "_", getTypeFromToken(&token, &symStack), argumentNumber);
        if (result)
            return result;
        getTokenWrapped();
        return parList();
    }
    return printErrorAndReturn("Syntax error has occured in parItem", SYNTAX_ERR);
}

int parList() {
    if (token.type == T_COMMA) {
        getTokenWrapped();
        argumentNumber++;
        return parItem();       
    } else if (token.type == T_CL_PAR) {
        int result = 0;
        if ((result = argAmountCheck(&symStack, funName.str, argumentNumber)) != 0)
            return result;
        argumentNumber=0;
        getTokenWrapped();
        return NO_ERR;
    }
    return printErrorAndReturn("Syntax error has occured in parList", SYNTAX_ERR);
    
}

int parListId() {
    int result = 0;
    if ( token.type == T_COLON ){ // ( ID : 
        getTokenWrapped();
        if (token.type == T_ID || litCheck()) { // ( ID : ID|LIT 
            if ( token.type == T_ID ) { 
                if ((result = checkInitialization(&symStack, token.content.str))) {
                    return result;
                }
            }
            result = checkArgument(&symStack, funName.str, argName.str, getTypeFromToken(&token, &symStack), argumentNumber);
            if (result)
                return result;
            getTokenWrapped();
            return parList();
        }
    } else if ( token.type == T_COMMA || token.type == T_CL_PAR) {
        if ((result = checkInitialization(&symStack, argName.str))){
            return result;
        }
        if (symtable_stack_search(&symStack, argName.str) == NULL )
            return printErrorAndReturn("Entrenal error in parListID", ERROR_INTERNAL);
        result = checkArgument(&symStack, funName.str, "_", (symtable_stack_search(&symStack, argName.str))->type, argumentNumber);
        if (result)
                return result;
        return parList();
    }
    return printErrorAndReturn("Syntax error has occured in parListId", SYNTAX_ERR);
}

int funCall(int  *type) {
    int result = 0;
    getTokenWrapped();
    if (checkDefinition(&symStack, funName.str))
        return SEM_ERR_UNDEFINED_FUNCTION;
    (*type) = symtable_stack_search(&symStack, funName.str)->type;
    if (token.type == T_CL_PAR) { // ()
        result =  zeroArgsCheck(&symStack, funName.str);
        if (result)
            return result;
        getTokenWrapped();
        return NO_ERR;
    } else if (token.type == T_ID) { // ( ID 
        str_copy_string(&argName, &(token.content));
        getTokenWrapped();
        return parListId();
    } else if (litCheck()) { // ( LIT
        result = checkArgument(&symStack, funName.str, "_", getTypeFromToken(&token, &symStack), argumentNumber);
        if (result)
            return result;
        getTokenWrapped();
        return parList();
    }
    return printErrorAndReturn("Syntax error has occured in funCall", SYNTAX_ERR);
}

int ifItem(){
    if (token.type == T_OP_BRACE) {
        getTokenWrapped();
        tFlagS(&token);
        int result = localParse();
        if (result)
            return result;
        if (token.type == T_ELSE) {
            getTokenWrapped();
            if (token.type == T_OP_BRACE) {
                getTokenWrapped();
                symtable_stack_push(&symStack);
                tFlagS(&token);
                return localParse();
            }
        }
    }
    return printErrorAndReturn("Syntaxe error in IfItem", SYNTAX_ERR);
}

int ifList(){
    int result = 0;
    int expType = ET_UNDEFINED;
    if (token.type != T_LET){
        int result = expAnalyse(&token, NULL, &expType, &symStack);
        symtable_stack_push(&symStack);
        if (result)
            return result;
        return ifItem();
    } else if (token.type == T_LET) {
        getTokenWrapped();
        if (token.type == T_ID) {
            result = checkInitialization(&symStack, token.content.str);
            if(result == 0) {
                symtable_stack_push(&symStack);
                int type = symtable_stack_search(&symStack, token.content.str)->type;
                if (type > ET_STRING)
                    type = type-3;
                varDefiner(&symStack, type, token.content.str, true, true);
                getTokenWrapped();
                return ifItem();
            }
            return result;
        }
    }
    return printErrorAndReturn("Syntaxe error in IfList", SYNTAX_ERR);
}

int whl() {
    int expType = ET_UNDEFINED;
    int result = expAnalyse(&token, NULL, &expType, &symStack);
    if(result) 
        return result;
    if (token.type == T_OP_BRACE) {
        getTokenWrapped();
        tFlagS(&token);
        symtable_stack_push(&symStack);
        return localParse();
    }
    return printErrorAndReturn("Syntaxe error in whl", SYNTAX_ERR);
}

int returnR () {
    int result = 0;
    // output_user_func_return(list);
    int expType = ET_VOID;
    if (token.type == T_ID || litCheck() || token.type == T_OP_PAR) {
        result =  expression(&expType);
    }
    if (result == 0)
        result = ReturnSemanticCheck(&symStack, funBodyName.str, expType);
    return result;
}

int parseInstruction() {
    if(token.type == T_LET || token.type == T_VAR) {
        return varDef();
    } else if (token.type == T_ID) {
        return id();
    } else if (token.type == T_FUNC) {
        return funDef();
    } else if (token.type == T_WHILE) {
        getTokenWrapped();
        return whl();
    } else if (token.type == T_IF) {
        getTokenWrapped();
        return ifList();
    } else if (token.type == T_RETURN) {
        if(!functionBodyFlag) {
            return printErrorAndReturn("Incorrect return usage, it must be within function body", SEM_ERR_OTHER);
        }
        getTokenWrapped();
        return returnR();
    }
    fprintf(stderr, "%d\n%s\n",token.type,token.content.str);
    return printErrorAndReturn("Syntaxe error in parseInstruction", SYNTAX_ERR);
}

int globalParse () {
    fprintf(stderr, "%s\n",token.content.str);
    if (token.type != T_EOF) {
        if (newLineCheck())
            return printErrorAndReturn("Syntax error has occured in globalParse, while newLineCheck", SYNTAX_ERR);
        int result = parseInstruction();
        if (result) 
            return result;
        return globalParse();
    } else {
        fprintf(stderr, "Success, EOF parsed\n");
        return NO_ERR;
    }
    return printErrorAndReturn("Syntax error has occured in globalParse", SYNTAX_ERR);
}

int localParse () {
    fprintf(stderr, "TOKEN TYPE is %d\n", token.type);
    if (token.type != T_CL_BRACE) {
        if (newLineCheck())
            return printErrorAndReturn("Syntax error has occured in localParse, while newLineCheck", SYNTAX_ERR);
        int result = parseInstruction();
        if (result) 
            return result;
        return localParse();
    } else {
        symtable_stack_pop(&symStack);
        fprintf(stderr, "Success, local parse ended\n");
        getTokenWrapped();
        return NO_ERR;
    }
    return printErrorAndReturn("Syntax error has occured in localParse", SYNTAX_ERR);
}

int functionParse () {
    fprintf(stderr, "TOKEN TYPE is %d\n", token.type);
    str_copy_string(&(funBodyName), &(funName));
    if (token.type != T_CL_BRACE) {
        if (newLineCheck())
            return printErrorAndReturn("Syntax error has occured in functionParse, while newLineCheck", SYNTAX_ERR);
        int result = parseInstruction();
        if (result) 
            return result;
        return localParse();
    } else {
        symtable_stack_pop(&symStack);
        functionBodyFlag = false;
        fprintf(stderr, "Success, function parse ended\n");
        getTokenWrapped();
        return NO_ERR;
    }
    return printErrorAndReturn("Syntax error has occured in functionParse", SYNTAX_ERR);
}

int first_analyse() {
    int result = 0;
    while (token.type != T_EOF && result == 0)
    {
        if (token.type == T_FUNC) {
            result = funDef();
        } else {
            getTokenWrapped();
        } 
    }
    if (fseek(stdin, 0L, SEEK_SET)) {
        perror("stdin");
        return(EXIT_FAILURE);
    }
    firstAnalyseFlag = false;
    functionBodyFlag = false;
    return NO_ERR;
}

int main() {
        int result = 0;
        initAll();
        // output_main_func(&list);
        symtable_stack_push(&symStack);
        buildInFunctionDefenition(&symStack);
        getTokenWrapped();
        tFlagS(&token);
        result = first_analyse();
        getTokenWrapped();
        tFlagS(&token);
        if(result == 0)
            result = globalParse();
        // if (!result)
        //     print_instruction(&list);
        freeAll();
        if (result){
            fprintf(stderr,"exit code in main is %d\n",result);
            exit(result);
        }
}