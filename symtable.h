#ifndef SYMTABLE_H
#define SYMTABLE_H

#include <stdio.h>
#include <string.h>

#define INIT_SIZE 8     //initial size for symtable
#define THRESHOLD 0.7   //if symtable is filled above this percentage, it is being resized twice its current size 

typedef struct {
    int smth;
}htab_data_t;

typedef struct htab_item{
    htab_data_t data;
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

void symtable_resize(symtable_t* table, int newTableSize);



#endif SYMTABLE_H
