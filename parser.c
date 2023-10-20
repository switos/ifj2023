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

int functionCall() {
    return 0;    
}

int expression() {
    if (token.type == T_ID || litCheck()) {
        fprintf(stderr, "Succes, expAnalyse is parsed\n");
        return expAnalyse(&token);
    } else if (token.type == T_FNAME) {
        return functionCall();
    }
    return printErrorAndReturn("Syntaxe error has occured in expression or function rule\n", SYNTAX_ERR);
}

int id() {
    //save id;
    getTokenWrapped();
    if (token.type == T_EQUAL) {
        getTokenWrapped();
        return expression();
    } else if (token.type = T_FNAME) {
        functionCall();
    }
    return printErrorAndReturn("Syntaxe error has occured in id state\n", SYNTAX_ERR);
    
}

int varDefItem() {
    if(token.type == T_EQUAL) {
            getTokenWrapped();
            return expression();
    } else {
        return NO_ERR;
    }
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


int parseInstruction() {
    if(token.type == T_LET || token.type == T_VAR) {
        return varDef();
    } else if (token.type == T_ID) {
        return id();
    } else {
        return printErrorAndReturn("Syntaxe error in parse rule", SYNTAX_ERR);
    }
}

int pasreGlobal () {
    getTokenWrapped();
    if (token.type != T_EOF) {
        int result = parseInstruction();
        if (result) 
            return result;
        return pasreGlobal();
    } else {
        fprintf(stderr, "Succes, EOF parsed\n");
        return NO_ERR;
    }
}

int main() {
        str_init(&token.content);
        int result = pasreGlobal();
        str_free(&token.content);
        if (result){
            fprintf(stderr,"exit code in main is %d\n",result);
            exit(result);
        }
}