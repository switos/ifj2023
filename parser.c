#include "parser.h"
token_t token;
precedenceStackNode_t* prcStack;
symtable_stack_t symStack;
string name;

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
    if (token.type == T_DOUBLE || token.type == T_STRING || token.type == T_INT) {
        return 1;
    } else {
        return 0;
    }
}

void exitAndFree(int etype) {
    str_free(&token.content);
    fprintf(stderr,"exit code is %d\n", etype);
    exit(etype);
}

void getTokenWrapped() {
    if (getToken(&token)) {
        exitAndFree(LEX_ERR); 
    }
}

int expression(int *expType) {
    if (litCheck()) {
        fprintf(stderr, "Success, expAnalyse is parsed\n");
        return expAnalyse(&token, NULL);
    } else if (token.type == T_ID) {
        token_t tmpToken;
        str_init(&(tmpToken.content));
        str_copy_string(&(tmpToken.content), &(token.content));
        tmpToken.type = token.type;
        getTokenWrapped();
        if( token.type == T_OP_PAR){
            return funCall();
        } else {
            int result = expAnalyse(&token, &tmpToken);
            str_free(&(tmpToken.content));
            return result;
        }
    }
    return printErrorAndReturn("Syntaxe error has occured in expression or function rule", SYNTAX_ERR);
}

int id() {
    //save id;
    getTokenWrapped();
    if (token.type == T_ASSING) {
        getTokenWrapped();
        int expType;
        return expression(&expType);
    } else if (token.type == T_OP_PAR) {
        return funCall();
    }
    fprintf(stderr,"%d\n", token.type);
    return printErrorAndReturn("Syntaxe error has occured in id", SYNTAX_ERR);
    
}

int varDefItem(bool modified, int type) {
    int result;
    if(token.type == T_ASSING) {
            getTokenWrapped();
            int expType = ET_UNDEFINED;
            if ((result = expression(&expType)))
                return result;
            if((result = VarDefAssignSemanticCheck(&type, expType)))
                return result;
            return NO_ERR;
    } else {
        return NO_ERR;
    }
    return printErrorAndReturn("Syntax error has occured in varDefItem", SYNTAX_ERR);
} 

int varDefList(bool modified) {
    if(token.type == T_COLON) {
            getTokenWrapped();
            if(typeCheck()) {
                getTokenWrapped();
                return varDefItem(modified, token.type);
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
    if (token.type == T_OP_BRACE) {
        getTokenWrapped();
        tFlagS(&token);
        symtable_stack_push(&symStack);
        return localParse();
    } else if (token.type == T_ARROW ){
        getTokenWrapped();
        if (typeCheck()) {
            getTokenWrapped();
            if (token.type == T_OP_BRACE) {
                getTokenWrapped();
                tFlagS(&token);
                symtable_stack_push(&symStack);
                return localParse();
            }
        }
    }
    return printErrorAndReturn("Syntax error has occured in funDefType", SYNTAX_ERR);
}

int funDefItem() {
    if (token.type == T_COMMA) {
        getTokenWrapped();
        if(token.type == T_ID || token.type == T_UNDER) {
            getTokenWrapped();
            if(token.type == T_ID || token.type == T_UNDER) {
                getTokenWrapped();
                if (token.type == T_COLON) {
                    getTokenWrapped();
                    if (typeCheck()) {
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
        getTokenWrapped();
        if(token.type == T_ID || token.type == T_UNDER) {
            getTokenWrapped();
            if (token.type == T_COLON) {
                getTokenWrapped();
                if (typeCheck()) {
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
        getTokenWrapped();
        if(token.type == T_OP_PAR) {
            getTokenWrapped();
            return funDefPlist();
        }
    }
    return printErrorAndReturn("Syntax error has occured in funDef", SYNTAX_ERR);
}
 
int parItem(){
    if (token.type == T_ID) {
        getTokenWrapped();
        return parListId();
    } else if (litCheck()) {
        getTokenWrapped();
        return parList();
    }
    return printErrorAndReturn("Syntax error has occured in parItem", SYNTAX_ERR);
}

int parList() {
    if (token.type == T_COMMA) {
        getTokenWrapped();
        return parItem();       
    } else if (token.type == T_CL_PAR) {
        getTokenWrapped();
        return NO_ERR;
    }
    return printErrorAndReturn("Syntax error has occured in parList", SYNTAX_ERR);
    
}

int parListId() {
    if (token.type == T_COLON ){
        getTokenWrapped();
        if (token.type == T_ID || litCheck()) {
            getTokenWrapped();
            return parList();
        }
    } else if ( token.type == T_COMMA || token.type == T_CL_PAR) {
        return parList();
    }
    return printErrorAndReturn("Syntax error has occured in parListId", SYNTAX_ERR);
}

int funCall() {
    getTokenWrapped();
    if (token.type == T_CL_PAR) {
        getTokenWrapped();
        return NO_ERR;
    } else if (token.type == T_ID) {
        getTokenWrapped();
        return parListId();
    } else if (litCheck()) {
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
    if (token.type == T_OP_PAR){
        int result = expAnalyse(&token, NULL);
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
    if (token.type == T_OP_PAR) { 
        int result = expAnalyse(&token, NULL);
        if(result) 
            return result;
        if (token.type == T_OP_BRACE) {
            getTokenWrapped();
            tFlagS(&token);
            symtable_stack_push(&symStack);
            return localParse();
        }
    }
    return printErrorAndReturn("Syntaxe error in whl", SYNTAX_ERR);
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
        getTokenWrapped();
        int expType;
        return expression(&expType);
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

int main() {
        str_init(&token.content);
        str_init(&name);
        symtable_stack_init(&symStack);
        symtable_stack_push(&symStack);
        getTokenWrapped();
        tFlagS(&token);
        int result = globalParse();
        symtable_stack_free(&symStack);
        str_free(&token.content);
        str_free(&name);
        if (result){
            fprintf(stderr,"exit code in main is %d\n",result);
            exit(result);
        }
}