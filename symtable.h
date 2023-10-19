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
    char* type;
}data_param_t;

typedef struct {
    char* type;
    char* name;
    bool defined;
    bool constant;
    int argumentAmount;
    int argumentsInArray;
    data_param_t* param;
}htab_data_t;

typedef struct htab_item{
    htab_data_t* data;
    struct htab_item* next;
    char* key;
}htab_item_t;

typedef struct {
    htab_item_t** bucket;
    int sizeAllocated;
    int sizeUsed;
}symtable_t;

unsigned int get_hash (char *key, int tableSize);

symtable_t* symtable_init();

symtable_t* symtable_resize(symtable_t* table, int newTableSize);

htab_data_t* symtable_insert_data(symtable_t* table, char* key, char* type, char* name, bool defined, bool constant, int argumentAmount);

bool symtable_add_arguments(htab_data_t* func, char* name, char* identifier, char* type);

void symtable_free (symtable_t* table);

htab_data_t* symtable_search (symtable_t* table, char* key);

#endif