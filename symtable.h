#ifndef SYMTABLE_H
#define SYMTABLE_H


#include "stdbool.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define INIT_SIZE 8     //initial size for symtable
#define THRESHOLD 0.7   //if symtable is filled above this percentage, it is being resized twice its current size 

typedef struct {
    char* name;
    char* identifier;
    int type;
}data_param_t;

typedef struct {
    int type;
    char* name;
    bool initialized;
    bool constant;
    bool nil;
    int argumentAmount;
    int argumentsInArray;
    data_param_t** param;
}htab_data_t;

typedef struct htab_item{
    htab_data_t* data;
    char* key;
}htab_item_t;

typedef struct {
    htab_item_t** array;
    int sizeAllocated;
    int sizeUsed;
}symtable_t;

typedef struct stack_level {
    symtable_t* table;
    struct stack_level* next;
    struct stack_level* prev;
}stack_level_t;

typedef struct symtable_stack {
    stack_level_t* top;
}symtable_stack_t;

unsigned int get_hash (char *key, int tableSize);

symtable_t* symtable_init();

symtable_t* symtable_resize(symtable_t** table, int newTableSize);

htab_data_t* symtable_insert_data(symtable_t** table, char* key, int type, char* name, bool initialized, bool constant, int argumentAmount);

bool symtable_add_arguments(htab_data_t** func, char* name, char* identifier, int type);

void symtable_free (symtable_t* table);

htab_data_t* symtable_search (symtable_t* table, char* key);

symtable_t* symtable_get_global(symtable_stack_t* stack);

void symtable_set_nil (symtable_t* table, char* key, bool nil);

void symtable_stack_init (symtable_stack_t* stack);

void symtable_stack_free (symtable_stack_t* stack);

stack_level_t* symtable_stack_push (symtable_stack_t* stack);

void symtable_stack_pop (symtable_stack_t* stack);

#endif