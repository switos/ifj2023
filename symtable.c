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

    table->array = (htab_item_t**)calloc(INIT_SIZE, sizeof(htab_item_t*));
    if(table->array == NULL) {
        fprintf(stderr, "Failed to allocate memory");
    }

    for(int i = 0; i < INIT_SIZE; i++) {
        table->array[i] = NULL;
    }
    return table;
}

symtable_t* symtable_resize(symtable_t* table, int newTableSize) {
    htab_item_t** newArray = (htab_item_t**)calloc(newTableSize, sizeof(htab_item_t*));
    if (newArray == NULL) {
        fprintf(stderr, "Failed to allocate memory");
        return NULL;
    }

    for (int i = 0; i < table->sizeAllocated; i++) {
        htab_item_t* tmp = table->array[i];   
        unsigned int index = get_hash(tmp->key, newTableSize);
            
        while (newArray[index] != NULL) {
            index = (index + 1) % newTableSize;
        }
        newArray[index] = tmp;
        
    }
    
    symtable_t* newTable = (symtable_t*)malloc(sizeof(symtable_t));
    if (newTable == NULL) {
        fprintf(stderr, "Failed to allocate memory");
        free(newArray);
        return NULL;
    }

    newTable->sizeAllocated = newTableSize;
    newTable->sizeUsed = table->sizeUsed;
    newTable->array = newArray;

    return newTable;
}

htab_data_t* symtable_insert_data(symtable_t* table, char* key, char* type, char* name, bool defined, bool constant, int argumentAmount) {
    if (table->sizeUsed / table->sizeAllocated > THRESHOLD) {
        table = symtable_resize(table, table->sizeAllocated * 2);
    }
    
    unsigned int index = get_hash(key, table->sizeAllocated);
    unsigned int original_index = index;
    unsigned int probes = 0;

    htab_item_t* newSlot = (htab_item_t*)malloc(sizeof(htab_item_t));
    if (newSlot == NULL) {
        fprintf(stderr, "Failed to allocate memory");
        return NULL;
    }

    newSlot->key = (char*)malloc(strlen(key) + 1);
    newSlot->data = (htab_data_t*)malloc(sizeof(htab_data_t));
    newSlot->data->name = (char*)malloc(strlen(name) + 1);
    newSlot->data->type = (char*)malloc(strlen(type) + 1);

    if (newSlot->key == NULL || newSlot->data == NULL || newSlot->data->name == NULL || newSlot->data->type == NULL) {
        fprintf(stderr, "Failed to allocate memory");
        free(newSlot->key);
        free(newSlot->data->name);
        free(newSlot->data->type);
        free(newSlot->data);
        free(newSlot);
        return NULL;
    }

    if (argumentAmount != 0) {
        newSlot->data->param = (data_param_t*)malloc(sizeof(data_param_t) * argumentAmount);
        if (newSlot->data->param == NULL) {
            fprintf(stderr, "Failed to allocate memory");
            free(newSlot->key);
            free(newSlot->data->name);
            free(newSlot->data->type);
            free(newSlot->data);
            free(newSlot);
            return NULL;
        }
    }

    strcpy(newSlot->key, key);
    strcpy(newSlot->data->name, name);
    strcpy(newSlot->data->type, type);

    newSlot->data->argumentAmount = argumentAmount;
    newSlot->data->argumentsInArray = 0;
    newSlot->data->constant = constant;
    newSlot->data->defined = defined;

    while (table->array[index] != NULL) {
        index = (index + 1) % table->sizeAllocated;
        probes++;

        if (probes >= table->sizeAllocated) {
            fprintf(stderr, "Hash table is full.");
            free(newSlot->key);
            free(newSlot->data->name);
            free(newSlot->data->type);
            free(newSlot->data);
            free(newSlot);
            return NULL;
        }
    }

    table->array[index] = newSlot;
    table->sizeUsed++;

    return newSlot->data;
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
htab_data_t* symtable_search(symtable_t* table, char* key) {
    if (table == NULL || key == NULL) {
        return NULL;
    }

    unsigned int index = get_hash(key, table->sizeAllocated);
    unsigned int original_index = index;

    while (table->array[index] != NULL) {
        htab_item_t* tmp = table->array[index];

        if (tmp->key != NULL && !strcmp(tmp->key, key)) {
            printf("Got it\n");
            return &tmp->data;
        }

        index = (index + 1) % table->sizeAllocated;
        if (index == original_index) {
            break;
        }
    }

    return NULL;
}

void symtable_free(symtable_t* table) {
    if (table == NULL) {
        return;
    }

    for (int i = 0; i < table->sizeAllocated; i++) {
        htab_item_t* tmp = table->array[i];

        if (tmp->data != NULL) {

            if (tmp->data->argumentsInArray > 0) {
                for (int j = 0; j < tmp->data->argumentsInArray; j++) {
                    free(tmp->data->param[j].identifier);
                    free(tmp->data->param[j].name);
                    free(tmp->data->param[j].type);
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
    
    free(table->array);
    free(table);
}
