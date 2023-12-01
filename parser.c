#include "parser.h"
token_t token;
precedenceStackNode_t* prcStack;
symtable_stack_t symStack;
string name;
string argName;
string argId;
int argumentNumber = 0;
bool firstAnalyseFlag = true;
bool functionBodyFlag = false;

int newLineCheck() {
    if (token.newLineFlag == false)
        return 1;
    return 0;
}

int litCheck(){
    if (token.type == T_FLOAT_LIT || token.type == T_STRING_LIT || token.type == T_INT_LIT) 
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
    symtable_stack_free(&symStack);
    str_free(&token.content);
    str_free(&name);
    str_free(&argName);
    str_free(&argId);
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
            str_copy_string(&name, &(tmpToken.content));
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
    str_copy_string(&name, &(token.content)); //save id
    getTokenWrapped();
    if ((result = idCheck(&symStack, name.str))) 
        return result;
    if (token.type == T_ASSING) {
        getTokenWrapped();
        result = expression(&expType);
        if (result == 0)
            result = idCheckType(&symStack, name.str, expType);
        return result;
    } else if (token.type == T_OP_PAR) {
        result = funCall(&expType);
        if (result == 0)
            result = idCheckType(&symStack, name.str, expType);
        return result;
    }
    return printErrorAndReturn("Syntaxe error has occured in id", SYNTAX_ERR);
}

int varDefItem(bool modified, int type) {
    int typetmp = type;
    int result;
    if(token.type == T_ASSING) {
            getTokenWrapped();
            int expType = ET_UNDEFINED;
            if ((result = expression(&expType)))
                return result;
            fprintf(stderr,"TOKEN TYPE %d %d name is %s\n", type, expType, name.str);
            if((result = VarDefAssignSemanticCheck(&typetmp, expType)))
                return result;
            varDefiner(&symStack, typetmp, name.str, true, modified);
            if (symtable_stack_search(&symStack, name.str) == NULL) {
                fprintf(stderr,"Var definer dont work in varDefItem\n");
            }
            return NO_ERR;
    } else {
        varDefiner(&symStack, typetmp, name.str, false, modified);
        return NO_ERR;
    }
    return printErrorAndReturn("Syntax error has occured in varDefItem", SYNTAX_ERR);
} 

int varDefList(bool modified) {
    if(token.type == T_COLON) {
        getTokenWrapped();
        if(typeCheck()) {
            int type = token.type;
            fprintf(stderr,"TOKEN TYPE %d \n",token.type);
            getTokenWrapped();
            return varDefItem(modified, type);
        }
    } else if(token.type == T_ASSING) {
        return varDefItem(modified, ET_UNDEFINED);
    }
    return printErrorAndReturn("Syntaxe error in VarDefList", SYNTAX_ERR);
}

int varDef() {
    bool modified;
    if(token.type == T_LET) {
        modified = true;
    } else {
        modified = false;
    }
    getTokenWrapped();
    if(token.type ==  T_ID) {
        str_copy_string(&name, &(token.content));
        if(symtable_search(symStack.top->table, name.str) != NULL)
            return printErrorAndReturn("Sematic error in VarDef, redefinition", SEM_ERR_UNDEFINED_FUNCTION);
        getTokenWrapped();
        return varDefList(modified);
    }
    return printErrorAndReturn("Syntaxe error in VarDef", SYNTAX_ERR);
}

int funDefType() {
    functionBodyFlag = true;
    if (token.type == T_OP_BRACE) {
        getTokenWrapped();
        setType(&symStack, name.str, ET_VOID);
        tFlagS(&token);
        symtable_stack_push(&symStack);
        if (firstAnalyseFlag) 
            return NO_ERR;  
        return localParse();
    } else if (token.type == T_ARROW ){
        getTokenWrapped();
        if (typeCheck()) {
            setType(&symStack, name.str, token.type);
            getTokenWrapped();
            if (token.type == T_OP_BRACE) {
                getTokenWrapped();
                tFlagS(&token);
                symtable_stack_push(&symStack);
                if (firstAnalyseFlag) 
                    return NO_ERR;  
                return localParse();
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
                        result = funAddArgument(&symStack, name.str, argName.str, argId.str, token.type);
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
                    funAddArgument(&symStack, name.str, argName.str, argId.str, token.type);
                    getTokenWrapped();
                    return funDefItem();
                }
            }
        }
    }
    return printErrorAndReturn("Syntaxe error has occured in funDefPlist", SYNTAX_ERR);
}

int funDef() {
    getTokenWrapped();
    if (token.type == T_ID) {   
        str_copy_string(&name, &(token.content)); //save id
        getTokenWrapped();
        if(token.type == T_OP_PAR) {  
            getTokenWrapped();
            if (funDefiner(&symStack, ET_UNDEFINED, name.str))
                return SEM_ERR_UNDEFINED_FUNCTION;
            return funDefPlist();
        }
    }
    return printErrorAndReturn("Syntax error has occured in funDef", SYNTAX_ERR);
}
 
int parItem(){
    int result = 0;
    if (token.type == T_ID) {
        str_copy_string(&argName, &(token.content));
        getTokenWrapped();
        return parListId();
    } else if (litCheck()) {
        result = checkArgument(&symStack, name.str, "_", getTypeFromToken(&token, &symStack), argumentNumber);
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
        argumentNumber=0;
        getTokenWrapped();
        return NO_ERR;
    }
    return printErrorAndReturn("Syntax error has occured in parList", SYNTAX_ERR);
    
}

int parListId() {
    int result = 0;
    if (token.type == T_COLON ){
        getTokenWrapped();
        if (token.type == T_ID || litCheck()) {
            if ( token.type == T_ID ) {
                if ((result = checkInitialization(&symStack, argName.str))){
                    return result;
                }
            }
            result = checkArgument(&symStack, name.str, argName.str, getTypeFromToken(&token, &symStack), argumentNumber);
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
        result = checkArgument(&symStack, name.str, "_", (symtable_stack_search(&symStack, argName.str))->type, argumentNumber);
        if (result)
                return result;
        return parList();
    }
    return printErrorAndReturn("Syntax error has occured in parListId", SYNTAX_ERR);
}

int funCall(int  *type) {
    int result = 0;
    getTokenWrapped();
    if (checkDefinition(&symStack, name.str))
        return SEM_ERR_UNDEFINED_FUNCTION;
    (*type) = symtable_stack_search(&symStack, name.str)->type;
    if (token.type == T_CL_PAR) {
        result =  zeroArgsCheck(&symStack, name.str);
        if (result)
            return result;
        getTokenWrapped();
        return NO_ERR;
    } else if (token.type == T_ID) {
        str_copy_string(&argName, &(token.content));
        getTokenWrapped();
        return parListId();
    } else if (litCheck()) {
        result = checkArgument(&symStack, name.str, "_", getTypeFromToken(&token, &symStack), argumentNumber);
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
        symtable_stack_push(&symStack);
        int result = localParse();
        if (result)
            return result;
        if (token.type == T_ELSE) {
            getTokenWrapped();
            if (token.type == T_OP_BRACE) {
                getTokenWrapped();
                tFlagS(&token);
                symtable_stack_push(&symStack);
                return localParse();
            }
        }
    }
    return printErrorAndReturn("Syntaxe error in IfItem", SYNTAX_ERR);
}

int ifList(){
    int expType = ET_UNDEFINED;
    if (token.type != T_LET){
        int result = expAnalyse(&token, NULL, &expType, &symStack);
        if (result)
            return result;
        return ifItem();
    } else if (token.type == T_LET) {
        getTokenWrapped();
        if (token.type == T_ID) {
            getTokenWrapped();
            return ifItem();
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
    int expType;
    if (token.type == T_ID || litCheck() || token.type == T_OP_PAR) {
        return expression(&expType);
    }
    return NO_ERR;
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
        //check if we are in function defenition scope
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
        fprintf(stderr, "Succes, EOF parsed\n");
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
        fprintf(stderr, "Succes, RIGHT CURVY HORE parsed\n");
        getTokenWrapped();
        return NO_ERR;
    }
    return printErrorAndReturn("Syntax error has occured in localParse", SYNTAX_ERR);
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
        str_init(&token.content);
        str_init(&name);
        str_init(&argName);
        str_init(&argId);
        symtable_stack_init(&symStack);
        symtable_stack_push(&symStack);
        getTokenWrapped();
        tFlagS(&token);
        result = first_analyse();
        getTokenWrapped();
        tFlagS(&token);
        if(result == 0)
            result = globalParse();
        symtable_stack_free(&symStack);
        str_free(&token.content);
        str_free(&name);
        str_free(&argName);
        str_free(&argId);
        if (result){
            fprintf(stderr,"exit code in main is %d\n",result);
            exit(result);
        }
}