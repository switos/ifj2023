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
    fprintf(stderr,"exit code is %d\n",etype);
    exit(etype);
}

void getTokenWrapped() {
    if (getToken(&token)) {
        exitAndFree(LEX_ERR); 
    }
}

int assigmentRule() {
    if (litCheck() || token.type == T_ID) {
        int result = expAnalyse(&token);
        if(result)
            return result;
        fprintf(stderr, "Succes, assigments rule is parsed\n");
        getTokenWrapped();
        return parse();
    } else  {
        return printErrorAndReturn("Syntaxe error in assigment rule", SYNTAX_ERR);
    }
            
}

int varDefTypeInitRule() {
    if(token.type == T_DOUBLE || token.type == T_INT || token.type == T_STRING) {
        getTokenWrapped();
        if(token.type == T_EQUAL) {
            getTokenWrapped();
            return assigmentRule();
        } else {
            return parse();
        }
    } else {
        return printErrorAndReturn("Syntaxe error in varDefTypeInitRule", SYNTAX_ERR);
    }
} 

int variableDefRule() {
    if(token.type == T_LET) {
        //int varTypeTmp = UNMUTABLE_T
    } else {
        //int varTypeTmp = MUTABLE_T
    }
    getTokenWrapped();
    if(token.type ==  T_ID) {
        getTokenWrapped();
        if(token.type == T_COLON) {
            getTokenWrapped();
            return varDefTypeInitRule();
        } else if(token.type == T_EQUAL) {
            getTokenWrapped();
            return assigmentRule();
        } 
    }
    return printErrorAndReturn("Syntaxe error in variableDefRule", SYNTAX_ERR);

}

int parse() {
    if(token.type == T_LET || token.type == T_VAR) {
        return variableDefRule();
    } else if(token.type == T_EOF) {
        fprintf(stderr, "Succes, EOF parsed\n");
        return NO_ERR;
    } else {
        return printErrorAndReturn("Syntaxe error in parse rule", SYNTAX_ERR);
    }
}

int main() {
        str_init(&token.content);
        getTokenWrapped();
        int result = parse();
        str_free(&token.content);
        if (result){
            fprintf(stderr,"exit code is %d\n",result);
            exit(result);
        }
}