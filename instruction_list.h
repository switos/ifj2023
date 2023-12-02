#ifndef INSTRUCTION_LIST
#define INSTRUCTION_LIST

#include <stdio.h>
#include <stdbool.h>

#include "scanner.h"
#include "symtable.h"
#include "str.h"
#include "expressionAnalyser.h"

typedef enum {
    I_MOVE,
    I_CREATEFRAME,
    I_PUSHFRAME,
    I_POPFRAME,
    I_DEFVAR,
    I_CALL,
    I_RETURN,

    I_PUSHS,
    I_POPS,
    I_CLEARS,

    I_ADD,
    I_SUB,
    I_MUL,
    I_DIV,
    I_IDIV,

    I_ADDS,
    I_SUBS,
    I_MULS,
    I_DIVS,
    I_IDIVS,

    I_LT,
    I_GT,
    I_EQ,
    I_LTS,
    I_GTS,
    I_EQS,

    I_AND,
    I_OR,
    I_NOT,
    I_ANDS,
    I_ORS,
    I_NOTS,

    I_INT2FLOAT,
    I_FLOAT2INT,
    I_INT2CHAR,
    I_STRI2INT,
    I_INT2FLOATS,
    I_FLOAT2INTS,
    I_INT2CHARS,
    I_STRI2INTS,

    I_READ,
    I_WRITE,

    I_CONCAT,
    I_STRLEN,
    I_GETCHAR,
    I_SETCHAR,

    I_TYPE,

    I_LABEL,
    I_JUMP,
    I_JUMPIFEQ,
    I_JUMPIFNEQ,
    I_JUMPIFEQS,
    I_JUMPIFNEQS,
    I_EXIT,

    I_BREAK,
    I_DPRINT,

    I_COMMENT,

    I_DEFAULT,

} instructions;

typedef enum {
    F_DEFAULT,
    F_GF,       //global frame
    F_LF,       //local frame
    F_TF,       //temporary frame   
} frame_type;

typedef struct {
    string out;
    char* value;
    ET_TYPE type;
    frame_type frame;
} operand_t;

typedef struct {
    operand_t* operand_1;
    operand_t* operand_2;
    operand_t* result;
    instructions inst;
} taCode;

typedef struct DLLElement{
    taCode* data;
    struct DLLElement *previousElement;
    struct DLLElement *nextElement;
} *DLLElementPtr;

typedef struct {
    DLLElementPtr firstElement;
    DLLElementPtr lastElement;
    DLLElementPtr activeElement;
} DLList;

/**
 * @brief initing the list
 * 
 * @param list pointer to list
 */
void DLL_Init(DLList* list);

/**
 * @brief eliminating the list
 * 
 * @param list pointer to list
 */
void DLL_Dispose(DLList* list);

/**
 * @brief inserting new element on first position in list
 * 
 * @param list pointer to the list
 * @param data pointer to data of new element
 */
void DLL_InsertFirst(DLList* list, taCode* data);

/**
 * @brief inserting new element on last position in list
 * 
 * @param list pointer to the list
 * @param data pointer to data of new element
 */
void DLL_InsertLast(DLList* list, taCode* data);

/**
 * @brief setting first element of the list as active
 * 
 * @param list pointer to the list
 */
void DLL_First(DLList* list);

/**
 * @brief setting last element of the list as active
 * 
 * @param list pointer to the list
 */
void DLL_Last(DLList* list);

/**
 * @brief getting value of the first element of the list
 * 
 * @param list pointer to list
 * @param dataPtr pointer to the returning data
 */
void DLL_GetFirst(DLList* list, taCode* dataPtr);

/**
 * @brief getting value of the last element of the list
 * 
 * @param list pointer to the list
 * @param dataPtr pointer to the returning data
 */
void DLL_GetLast(DLList* list, taCode* dataPtr);

/**
 * @brief deleting first element of the list
 * 
 * @param list pointer to the list
 */
void DLL_DeleteFirst(DLList* list);

/**
 * @brief deleting last element of the list
 * 
 * @param list pointer to the list
 */
void DLL_DeleteLast(DLList* list);

/**
 * @brief deleting an element after active
 * 
 * @param list pointer to the list
 */
void DLL_DeleteAfter(DLList* list);

/**
 * @brief deleting an element before active
 * 
 * @param list pointer to the list
 */
void DLL_DeleteBefore(DLList* list);

/**
 * @brief inserting an element after active
 * 
 * @param list pointer to the list
 * @param data pointer to data of new element
 */
void DLL_InsertAfter(DLList* list, taCode* data);

/**
 * @brief inserting an element before active
 * 
 * @param list pointer to the list
 * @param data pointer to data of new element
 */
void DLL_InsertBefore(DLList* list, taCode* data);

/**
 * @brief getting value of the active element
 * 
 * @param list pointer to the list
 * @param dataPtr pointer to the returning data
 */
void DLL_GetValue(DLList* list, taCode* dataPtr);

/**
 * @brief setting value to the active element
 * 
 * @param list pointer to the list
 * @param data pointer to the insetrting data
 */
void DLL_SetValue(DLList* list, taCode* data);

/**
 * @brief changing activity to the next element
 * 
 * @param list pointer to the list
 */
void DLL_Next(DLList* list);

/**
 * @brief changing activity to the previous element
 * 
 * @param list pointer to the list
 */
void DLL_Previous(DLList* list);

/**
 * @brief cheking if there is an active element in the list
 * 
 * @param list pointer to the list
 * @return 1 if there is active element
 * @return 0 if there is not active element 
 */
int DLL_IsActive(DLList* list);

/**
 * @brief setting value of the operand
 * 
 * @param target pointer to the data
 * @param source string  to be written in target
 * @return true if successful
 * @return false if not successful
 */
bool set_operand_value(operand_t* target, char* source);

/**
 * @brief eliminating data
 * 
 * @param target pointer to the data
 */
void free_data_value(taCode* target);

/**
 * @brief writing new data
 * 
 * @param target pointer to the data
 * @param source data to be written in target
 * @return true if successful
 * @return false if not successful
 */
bool insert_data(taCode* target, taCode* source);

/**
 * @brief returning data to default parameters 
 * 
 * @param target pointer to the data
 */
void clear_data(taCode* source);

#endif