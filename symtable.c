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

htab_data_t* symtable_insert_variable(symtable_t* table, char* key, char* type, char* name, char* value, bool defined, bool constant){
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

    newBucket->data->var = (data_variable_t*)malloc(sizeof(data_variable_t));
    if(newBucket->data->var == NULL) {
        fprintf(stderr, "Failed to allocate memory");
        free(newBucket->data); 
        free(newBucket);
        return NULL; 
    }

    newBucket->data->var->name = (char*)malloc(strlen(name) + 1);
    if(newBucket->data->var->name == NULL) {
        fprintf(stderr, "Failed to allocate memory");
        free(newBucket->data->var);
        free(newBucket->data); 
        free(newBucket);
        return NULL; 
    }

    newBucket->data->var->type = (char*)malloc(strlen(type) + 1);
    if(newBucket->data->var->type == NULL) {
        fprintf(stderr, "Failed to allocate memory");
        free(newBucket->data->var->name);
        free(newBucket->data->var);
        free(newBucket->data); 
        free(newBucket);
        return NULL; 
    }
    
    newBucket->data->var->value = (char*)malloc(strlen(value) + 1);
    if(newBucket->data->var->value == NULL) {
        fprintf(stderr, "Failed to allocate memory");
        free(newBucket->data->var->type);
        free(newBucket->data->var->name);
        free(newBucket->data->var);
        free(newBucket->data); 
        free(newBucket);
        return NULL; 
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
        table = symtable_resize(table, table->sizeAllocated * 2);
    }

    unsigned int index = get_hash(key, table->sizeAllocated);

    htab_item_t* newBucket = (htab_item_t*)malloc(sizeof(htab_item_t));
    if(newBucket == NULL) {
        fprintf(stderr, "Failed to allocate memory");
    }
    newBucket->data = (htab_data_t*)malloc(sizeof(htab_data_t));
    if(newBucket->data == NULL) {
        fprintf(stderr, "Failed to allocate memory");
        free(newBucket); 
        return NULL; 
    }

    newBucket->data->func = (data_func_t*)malloc(sizeof(data_func_t));
    if(newBucket->data->func == NULL) {
        fprintf(stderr, "Failed to allocate memory");
        free(newBucket->data); 
        free(newBucket);
        return NULL; 
    }

    newBucket->data->func->name = (char*)malloc(strlen(name) + 1);
    if(newBucket->data->func->name == NULL) {
        fprintf(stderr, "Failed to allocate memory");
        free(newBucket->data->func);
        free(newBucket->data); 
        free(newBucket);
        return NULL; 
    }

    newBucket->data->func->returnType = (char*)malloc(strlen(returnType) + 1);
    if(newBucket->data->func->returnType == NULL) {
        fprintf(stderr, "Failed to allocate memory");
        free(newBucket->data->func->name);
        free(newBucket->data->func);
        free(newBucket->data); 
        free(newBucket);
        return NULL; 
    }

    newBucket->data->func->param = (data_param_t*)malloc(sizeof(data_param_t) * argumentAmount);
    if(newBucket->data->func->param == NULL) {
        fprintf(stderr, "Failed to allocate memory");
        free(newBucket->data->func->returnType);
        free(newBucket->data->func->name);
        free(newBucket->data->func);
        free(newBucket->data); 
        free(newBucket);
        return NULL; 
    }

    strcpy(newBucket->data->func->name, name);
    strcpy(newBucket->data->func->returnType, returnType);
    newBucket->data->func->defined = defined;
    newBucket->data->func->argumentAmount = argumentAmount;
    newBucket->data->func->argumentsInArray = 0;

    table->bucket[index] = newBucket;
    table->sizeUsed++;

}

bool symtable_add_arguments(data_func_t* func, char* name, char* identifier, char* type) {
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
            if(tmp->data->var != NULL) {
                
                free(tmp->data->var->name);
                free(tmp->data->var->type);
                free(tmp->data->var->value);
                free(tmp->data->var);
            }
            if(tmp->data->func != NULL) {
                
                free(tmp->data->func->name);
                free(tmp->data->func->returnType);
                if(tmp->data->func->param != NULL) {
                    for(int i = 0; i < tmp->data->func->argumentsInArray; i++) {
                        free(tmp->data->func->param[i].identifier);
                        free(tmp->data->func->param[i].name);
                        free(tmp->data->func->param[i].type);
                    }
                    free(tmp->data->func->param);
                }
                free(tmp->data->func);
            }
            free(tmp);
            
        }
    }
    
    free(table->bucket);
    free(table);
}

int main(int argc, char* argv[]) {
    
    return 0;
}
