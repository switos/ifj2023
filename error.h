#ifndef ERROR_H
#define ERROR_H

#define NO_ERR                          0
#define LEX_ERR                         1
#define SYNTAX_ERR                      2
#define SEM_ERR_UNDEFINED_FUNCTION      3
#define SEM_ERR_WRONG_PARAM             4
#define SEM_ERR_UNDEFINED_VAR           5
#define SEM_ERR_WRONG_RET               6
#define SEM_ERR_TYPE_COMPAT             7
#define SEM_ERR_TYPE_GETTING            8
#define SEM_ERR_OTHER                   9
#define ERROR_INTERNAL                  99

int printErrorAndReturn(char * text, int code);

#endif