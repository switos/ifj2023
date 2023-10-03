#include "symtable.h"

//Division Hashing (Modulo Hashing)
unsigned int get_hash (char *key, int tableSize) {
    unsigned int hash;
    for(int i = 0; key[i] != '\0'; i++) {
        hash = (hash * 31 + key[i]) % tableSize;
    }
    return hash;
}

symtable_t* symtable_init() {

    symtable_t* table = (symtable_t*)malloc(sizeof(symtable_t));
    if(table == NULL) {
        fprintf(stderr, "Failed to allocate memory");
    }

    table->sizeAllocated = INIT_SIZE;
    table->sizeUsed = 0;

    table->bucket = (htab_item_t**)malloc(sizeof(htab_item_t*) * INIT_SIZE);
    if(table->bucket == NULL) {
        fprintf(stderr, "Failed to allocate memory");
    }

    for(int i = 0; i < INIT_SIZE; i++) {
        table->bucket[i] = NULL;
    }
    return table;
}

void symtable_resize(symtable_t* table, int newTableSize) {
   
}