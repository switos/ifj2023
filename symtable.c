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

    table->bucket = (htab_item_t**)calloc(INIT_SIZE, sizeof(htab_item_t*));
    if(table->bucket == NULL) {
        fprintf(stderr, "Failed to allocate memory");
    }

    for(int i = 0; i < INIT_SIZE; i++) {
        table->bucket[i] = NULL;
    }
    return table;
}

void symtable_resize(symtable_t* table, int newTableSize) {
   htab_item_t** newBucket = (htab_item_t**)calloc(newTableSize, sizeof(htab_item_t*));
   if(newBucket == NULL) {
        fprintf(stderr, "Failed to allocate memory");
    }

    for(int i = 0; i < table->sizeAllocated; i++) {
        htab_item_t* tmp = table->bucket[i];
        while(tmp != NULL) {
            htab_item_t* tmpNext = tmp->next;
            unsigned int index = get_hash(tmp->key, newTableSize);
            tmp->next = newBucket[index];
            newBucket[index] = tmp;
            tmp = tmpNext;
        }
    }
}

htab_data_t* symtable_insert_variable(symtable_t* table, char* key, char* type, char* name, char* value, bool defined, bool constant){
    if(table->sizeUsed / table->sizeAllocated > THRESHOLD) {
        resize(table, table->sizeAllocated * 2);
    }

    unsigned int index = get_hash(key, table->sizeAllocated);

    htab_item_t* newBucket = (htab_item_t*)malloc(sizeof(htab_item_t));
    if(newBucket == NULL) {
        fprintf(stderr, "Failed to allocate memory");
    }

    strcpy(newBucket->data->var->name, name);
    strcpy(newBucket->data->var->type, type);
    strcpy(newBucket->data->var->value, value);
    newBucket->data->var->defined = defined;
    newBucket->data->var->constant = constant;

    table->bucket[index] = newBucket;
    table->sizeUsed++;

}

htab_data_t* symtable_insert_func(symtable_t* table, char* key, char* returnType, char* name, bool defined, int argumentAmount){
    if(table->sizeUsed / table->sizeAllocated > THRESHOLD) {
        resize(table, table->sizeAllocated * 2);
    }

    unsigned int index = get_hash(key, table->sizeAllocated);

    htab_item_t* newBucket = (htab_item_t*)malloc(sizeof(htab_item_t));
    if(newBucket == NULL) {
        fprintf(stderr, "Failed to allocate memory");
    }

    strcpy(newBucket->data->func->name, name);
    strcpy(newBucket->data->func->returnType, returnType);
    newBucket->data->func->defined = defined;
    newBucket->data->func->argumentAmount = argumentAmount;
    newBucket->data->func->argumentsInArray = 0;
    newBucket->data->func->param = (data_param_t*)malloc(sizeof(data_param_t) * 2);
    if(newBucket->data->func->param == NULL) {
        fprintf(stderr, "Failed to allocate memory");
    }

    table->bucket[index] = newBucket;
    table->sizeUsed++;

}

bool symtable_add_arguments(data_func_t* func, char* name, char* identifier, char* type) {
    int pos = func->argumentsInArray;
    strcpy(func->param[pos].identifier, identifier);
    strcpy(func->param[pos].name, name);
    strcpy(func->param[pos].type, type);
}

htab_data_t* symtable_search (symtable_t* table, char* key) {
    if(table == NULL || key == NULL) {
        return NULL;
    }

    unsigned int index = get_hash(key, table->sizeAllocated);
    htab_item_t* tmp = table->bucket[index];

    while(tmp != NULL) {
        if(!strcmp(tmp->key, key)) {
            return &tmp->data;
        }
        tmp = tmp->next;
    }
}

void symtable_free (symtable_t* table) {
    for(int i = 0; i < table->sizeAllocated; i++) {
        htab_item_t* current = table->bucket[i];
        while(current != NULL) {
            htab_item_t* tmp = current;
            current = current->next;
            free(tmp);
        }
    }
    free(table->bucket);
    free(table);
}

