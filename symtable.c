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

symtable_t* symtable_resize(symtable_t* table, int newTableSize) {
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
    symtable_t* newTable = (symtable_t*)malloc(sizeof(symtable_t));
    if (newTable == NULL) {
        fprintf(stderr, "Failed to allocate memory");
        free(newBucket);
        return NULL;
    }

    newTable->sizeAllocated = newTableSize;
    newTable->sizeUsed = table->sizeUsed;
    newTable->bucket = newBucket;

    
    symtable_free(table);
    return newTable;
}

htab_data_t* symtable_insert_data(symtable_t* table, char* key, char* type, char* name, bool defined, bool constant, int argumentAmount) {
    if(table->sizeUsed / table->sizeAllocated > THRESHOLD) {
        table = symtable_resize(table, table->sizeAllocated * 2);
    }
    
    unsigned int index = get_hash(key, table->sizeAllocated);

    htab_item_t* newBucket = (htab_item_t*)malloc(sizeof(htab_item_t));
    if(newBucket == NULL) {
        fprintf(stderr, "Failed to allocate memory");
        return NULL;
    }

    newBucket->data = (htab_data_t*)malloc(sizeof(htab_data_t));
    if(newBucket->data == NULL) {
        fprintf(stderr, "Failed to allocate memory");
        free(newBucket); 
        return NULL; 
    }
    
    newBucket->key = (htab_data_t*)malloc(strlen(key) + 1);
    if(newBucket->key == NULL) {
        fprintf(stderr, "Failed to allocate memory");
        free(newBucket->data);
        free(newBucket); 
        return NULL; 
    }

    newBucket->data->name = (htab_data_t*)malloc(strlen(name) + 1);
    if(newBucket->data->name == NULL) {
        fprintf(stderr, "Failed to allocate memory");
        free(newBucket->data);
        free(newBucket->key);
        free(newBucket); 
        return NULL; 
    }

    newBucket->data->type = (htab_data_t*)malloc(strlen(type) + 1);
    if(newBucket->data->type == NULL) {
        fprintf(stderr, "Failed to allocate memory");
        free(newBucket->data->name);
        free(newBucket->data);
        free(newBucket->key);
        free(newBucket); 
        return NULL; 
    }

    if(argumentAmount != 0) {
        newBucket->data->param = (data_param_t*)malloc(sizeof(data_param_t) * argumentAmount);
        if(newBucket->data->param == NULL) {
        fprintf(stderr, "Failed to allocate memory");
        free(newBucket->data->name);
        free(newBucket->data->type);
        free(newBucket->data);
        free(newBucket->key);
        free(newBucket); 
        return NULL; 
        }
    }
    strcpy(newBucket->key, key);
    strcpy(newBucket->data->name, name);
    strcpy(newBucket->data->type, type);

    newBucket->data->argumentAmount = argumentAmount;
    newBucket->data->argumentsInArray = 0;
    newBucket->data->constant = constant;
    newBucket->data->defined = defined;

    return newBucket->data;
}


bool symtable_add_arguments(htab_data_t* func, char* name, char* identifier, char* type) {
    if(func->argumentsInArray >= func->argumentAmount) {
        return false;
    }
    int pos = func->argumentsInArray;
    
    func->param[pos].identifier = (char*)malloc(strlen(identifier) + 1);
    if(func->param[pos].identifier == NULL) {
        fprintf(stderr, "Failed to allocate memory");
        return false;
    }

    func->param[pos].name = (char*)malloc(strlen(name) + 1);
    if(func->param[pos].name == NULL) {
        fprintf(stderr, "Failed to allocate memory");
        free(func->param->identifier);
        return false;
    }

    func->param[pos].type = (char*)malloc(strlen(type) + 1);
    if(func->param[pos].type == NULL) {
        fprintf(stderr, "Failed to allocate memory");
        free(func->param->name);
        free(func->param->identifier);
        return false;
    }

    strcpy(func->param[pos].identifier, identifier);
    strcpy(func->param[pos].name, name);
    strcpy(func->param[pos].type, type);
    func->argumentsInArray++;
    return true;
}

htab_data_t* symtable_search (symtable_t* table, char* key) {
    if(table == NULL || key == NULL) {
        return NULL;
    }

    unsigned int index = get_hash(key, table->sizeAllocated);    
    htab_item_t* tmp = table->bucket[index];
    
    while(tmp != NULL) {
        if(tmp->key != NULL) {
            if(!strcmp(tmp->key, key)) {
                printf("Got it\n");
                return &tmp->data;
            }
        }
        
        tmp = tmp->next;
    }
    return NULL;
}

void symtable_free (symtable_t* table) {
    for(int i = 0; i < table->sizeAllocated; i++) {
        htab_item_t* current = table->bucket[i];
        while(current != NULL) {
            htab_item_t* tmp = current;
            current = current->next;
            if(tmp->data != NULL) {

                if(tmp->data->argumentsInArray > 0) {
                    for(int i = 0; i < tmp->data->argumentsInArray; i++) {
                        free(tmp->data->param[i].identifier);
                        free(tmp->data->param[i].name);
                        free(tmp->data->param[i].type);
                    }
                    free(tmp->data->param);
                }
                free(tmp->data->name);
                free(tmp->data->type);
                free(tmp->data);
            }
            free(tmp->key);
            free(tmp);
            
        }
    }
    
    free(table->bucket);
    free(table);
}