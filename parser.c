#include "parser.h"
token_t token;
precedenceStackNode_t* prcStack;

int litCheck(){
    if (token.type == T_FLOAT_LIT || token.type == T_STRING_LIT || token.type == T_INT_LIT) {
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

int expression() {
    if (token.type == T_ID || litCheck()) {
        fprintf(stderr, "Succes, expAnalyse is parsed\n");
        return expAnalyse(&token);
    } else if (token.type == T_FNAME) {
        return funCall();
    }
    return printErrorAndReturn("Syntaxe error has occured in expression or function rule", SYNTAX_ERR);
}

int id() {
    //save id;
    getTokenWrapped();
    if (token.type == T_EQUAL) {
        getTokenWrapped();
        return expression();
    } else if (token.type = T_LEFT_BRAC) {
        funCall();
    }
    return printErrorAndReturn("Syntaxe error has occured in id", SYNTAX_ERR);
    
}

int varDefItem() {
    if(token.type == T_EQUAL) {
            getTokenWrapped();
            return expression();
    } else {
        return NO_ERR;
    }
    return printErrorAndReturn("Syntax error has occured in varDefItem", SYNTAX_ERR);
} 

int varDefList() {
    if(token.type == T_COLON) {
            getTokenWrapped();
            if(token.type == T_DOUBLE || token.type == T_INT || token.type == T_STRING) {
                getTokenWrapped();
                return varDefItem();
            }
    } else if(token.type == T_EQUAL) {
            getTokenWrapped();
            return expression();
    } 
    return printErrorAndReturn("Syntaxe error in VarDefList", SYNTAX_ERR);
}

int varDef() {
    if(token.type == T_LET) {
        //int varTypeTmp = UNMUTABLE_T
    } else {
        //int varTypeTmp = MUTABLE_T
    }
    getTokenWrapped();
    if(token.type ==  T_ID) {
        getTokenWrapped();
        return varDefList();
    }
    return printErrorAndReturn("Syntaxe error in VarDef", SYNTAX_ERR);
}

int funDefType() {
    if (token.type == T_LEFT_CBRAC) {
        return localParse();
    } else if (token.type == T_ARROW ){
        getTokenWrapped();
        if (token.type == T_DOUBLE || token.type == T_INT || token.type == T_STRING) {
            getTokenWrapped();
            if (token.type == T_LEFT_CBRAC)
                return localParse();
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
                return funDefItem();
            }
        }
    } else if (token.type == T_RIGHT_BRAC) {
        getTokenWrapped();
        return funDefType();
    }
    return printErrorAndReturn("Syntaxe error has occured in funDefItem", SYNTAX_ERR);
}

int funDefPlist() {
    if (token.type == T_RIGHT_BRAC){
        getTokenWrapped();
        return funDefType();
    } else if(token.type == T_ID || token.type == T_UNDER) {
        getTokenWrapped();
        if(token.type == T_ID || token.type == T_UNDER) {
            getTokenWrapped();
            return funDefItem();
        }
    }
    return printErrorAndReturn("Syntaxe error has occured in funDefPlist", SYNTAX_ERR);
}

int funDef() {
    getTokenWrapped();
    if (token.type == T_ID) {
        getTokenWrapped();
        if(token.type == T_LEFT_BRAC) {
            getTokenWrapped();
            return funDefPlist();
        }
    }
    return printErrorAndReturn("Syntax error has occured in funDef", SYNTAX_ERR);
}
 
int ret() {
    getTokenWrapped();
    return expression();
}

int parItem(){
    if (token.type == T_ID || litCheck()) {
        getTokenWrapped();
        return parList();
    }
    return printErrorAndReturn("Syntax error has occured in parItem", SYNTAX_ERR);
}

int parList() {
    if (token.type == T_COLON) {
        getTokenWrapped();
        if (token.type == T_ID) {
            getTokenWrapped();
            return parList();
        }
    } else if (token.type == T_COMMA) {
        getTokenWrapped();
        return parItem();       
    } else if (token.type == T_RIGHT_BRAC) {
        return NO_ERR;
    }
    return printErrorAndReturn("Syntax error has occured in parList", SYNTAX_ERR);
    
}

int funCall() {
    getTokenWrapped();
    if (token.type == T_RIGHT_BRAC) {
        return NO_ERR;
    } else if (token.type == T_ID || litCheck()) {
        getTokenWrapped();
        return parList();
    }
    return printErrorAndReturn("Syntax error has occured in funCall", SYNTAX_ERR);
}

int parseInstruction() {
    if(token.type == T_LET || token.type == T_VAR) {
        return varDef();
    } else if (token.type == T_ID) {
        return id();
    } else if (token.type == T_FUNC) {
        return funDef();
    } else if (token.type == T_RETURN) {
        return ret();
    } 
    return printErrorAndReturn("Syntaxe error in parseInstruction", SYNTAX_ERR);
}

int globalParse () {
    getTokenWrapped();
    if (token.type != T_EOF) {
        int result = parseInstruction();
        if (result) 
            return result;
        return globalParse();
    } else {
        fprintf(stderr, "Succes, EOF parsed\n");
        return NO_ERR;
    }
}

int localParse () {
    getTokenWrapped();
    if (token.type != T_RIGHT_CBRAC) {
        int result = parseInstruction();
        if (result) 
            return result;
        return localParse();
    } else {
        fprintf(stderr, "Succes, RIGHT CURVY HORE parsed\n");
        return NO_ERR;
    }
}

int main() {
        str_init(&token.content);
        int result = globalParse();
        str_free(&token.content);
        if (result){
            fprintf(stderr,"exit code in main is %d\n",result);
            exit(result);
        }
}