
#include "symtable.h"

//Division Hashing (Modulo Hashing)
unsigned int get_hash (char *key, int tableSize) {
    unsigned int hash = 0;
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

symtable_t* symtable_resize(symtable_t** table, int newTableSize) {
    //printf("i am resized\n");
    htab_item_t** newArray = (htab_item_t**)calloc(newTableSize, sizeof(htab_item_t*));
    if (newArray == NULL) {
        fprintf(stderr, "Failed to allocate memory");
        return NULL;
    }
    
    for (int i = 0; i < (*table)->sizeAllocated; i++) {
        if((*table)->array[i] == NULL) {
            continue;
        }
        htab_item_t* tmp = (*table)->array[i];
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
    newTable->sizeUsed = (*table)->sizeUsed;
    newTable->array = newArray;

    return newTable;
}

htab_data_t* symtable_insert_data(symtable_t** table, char* key, int type, char* name, bool initialized, bool constant) {
    if ((*table)->sizeUsed / (*table)->sizeAllocated > THRESHOLD) {
        (*table) = symtable_resize( table, (*table)->sizeAllocated * 2);
    }
    
    unsigned int index = get_hash(key, (*table)->sizeAllocated);

    htab_item_t* newSlot = (htab_item_t*)malloc(sizeof(htab_item_t));
    if (newSlot == NULL) {
        fprintf(stderr, "Failed to allocate memory");
        return NULL;
    }
    
    newSlot->key = (char*)malloc(strlen(key) + 1);
    newSlot->data = (htab_data_t*)malloc(sizeof(htab_data_t));
    newSlot->data->name = (char*)malloc(strlen(name) + 1);

    if (newSlot->key == NULL || newSlot->data == NULL || newSlot->data->name == NULL) {
        fprintf(stderr, "Failed to allocate memory");
        free(newSlot->key);
        free(newSlot->data->name);
        free(newSlot->data);
        free(newSlot);
        return NULL;
    }

    strcpy(newSlot->key, key);
    strcpy(newSlot->data->name, name);

    newSlot->data->argumentAmount = 0;
    newSlot->data->argumentsInArray = 0;
    newSlot->data->constant = constant;
    newSlot->data->initialized = initialized;
    newSlot->data->type = type;
    newSlot->data->nil =false;

    do {
        index = (index + 1) % (*table)->sizeAllocated;
    } while((*table)->array[index]!=NULL);

    (*table)->array[index] = newSlot;
    (*table)->sizeUsed++;

    //printf("Inserted: key=%s, index=%u\n", key, index);

    return newSlot->data;
}

bool symtable_add_arguments(htab_data_t* func, char* name, char* identifier, int type) {
    if(func->argumentAmount == 0) {
        func->param = (data_param_t**)malloc(sizeof(data_param_t*));
    }
    else {
        func->param = (data_param_t**)realloc(func->param, sizeof(data_param_t) * (func->argumentAmount + 1));
    }
    func->argumentAmount++;
    int pos = func->argumentsInArray;
    
    func->param[pos] = (data_param_t*)malloc(sizeof(data_param_t));
    if (func->param[pos] == NULL) {
        fprintf(stderr, "Failed to allocate memory for parameter");
        return false;
    }
    
    func->param[pos]->identifier = (char*)malloc(strlen(identifier) + 1);
    if(func->param[pos]->identifier == NULL) {
        fprintf(stderr, "Failed to allocate memory");
        return false;
    }
    
    func->param[pos]->name = (char*)malloc(strlen(name) + 1);
    if(func->param[pos]->name == NULL) {
        fprintf(stderr, "Failed to allocate memory");
        free(func->param[pos]->identifier);
        return false;
    }
    
    func->param[pos]->type = type;

    strcpy(func->param[pos]->identifier, identifier);
    strcpy(func->param[pos]->name, name);
    
    func->argumentsInArray++;
    
    return true;
}

data_param_t* symtable_get_argument(symtable_t* table, char* key, int argumentPosition) {
    htab_data_t* func_data = symtable_search(table, key);
    fprintf(stderr, "name is %s, arguments amount %d, argument in array %d\n", func_data->name, func_data->argumentAmount, func_data->argumentsInArray);
    if (func_data == NULL) {
        fprintf(stderr, "func data is NULL\n");
        return NULL;
    }

    if (argumentPosition < 0 || argumentPosition > func_data->argumentAmount) {
        fprintf(stderr, "Problem is in get argument\n");
        return NULL;  // Invalid arguments or function not found.
    }

    return (func_data->param[argumentPosition]);
}

htab_data_t* symtable_search(symtable_t* table, char* key) {
    
    if (table == NULL || key == NULL) {
        fprintf(stderr, "table or key is NULL\n");
        return NULL;
    }

    unsigned int index = get_hash(key, table->sizeAllocated);
    unsigned int original_index = index;
    
    do {
        htab_item_t* tmp = table->array[index];

        if(tmp != NULL) {
            if (tmp->key != NULL) {
                if (strcmp(tmp->key, key) == 0) {
                    return tmp->data;
                }
            }
        }
        
        index = (index + 1) % table->sizeAllocated;
    } while(index != original_index);

    return NULL;
}

symtable_t* symtable_get_global(symtable_stack_t* stack) {
    if (stack == NULL || stack->top == NULL) {
        return NULL;  // Stack is empty.
    }

    stack_level_t* current = stack->top;

    while (current->prev != NULL) {
        current = current->prev;  // Move to the previous level in the stack.
    }

    return current->table;
}

void symtable_set_nil (symtable_t* table, char* key, bool nil) {
    htab_data_t* tmp_data = symtable_search(table, key);
    tmp_data->nil = nil;
}

void symtable_free(symtable_t* table) {
    if (table == NULL) {
        return;
    }

    for (int i = 0; i < table->sizeAllocated; i++) {
        if(table->array[i] == NULL) {
            continue;
        }
        htab_item_t* tmp = table->array[i];

        if (tmp->data != NULL) {
            
            if (tmp->data->argumentsInArray > 0) {
                for (int j = 0; j < tmp->data->argumentsInArray; j++) {
                    free(tmp->data->param[j]->identifier);
                    free(tmp->data->param[j]->name);
                    free(tmp->data->param[j]);
                }
                free(tmp->data->param);
            }
            
            free(tmp->data->name);
            free(tmp->data);
        }
        free(tmp->key);
        free(tmp);        
    }
    
    free(table->array);
    free(table);
}

void symtable_stack_init (symtable_stack_t* stack) {
    stack->top = NULL;
}

void symtable_stack_pop (symtable_stack_t* stack) {
    if (stack == NULL || stack->top == NULL) {
        return;
    }

    stack_level_t* current = stack->top;

    if (current->prev != NULL) {
        current->prev->next = NULL;
        stack->top = current->prev;
    }
    else {
        stack->top = NULL;
    }

    symtable_free(current->table);
    free(current);
}

void symtable_stack_free (symtable_stack_t* stack) {
    if (stack == NULL || stack->top == NULL) {
        return;
    }

    while(stack->top != NULL) {
        symtable_stack_pop(stack);
    }
}

stack_level_t* symtable_stack_push (symtable_stack_t* stack) {
    if(stack == NULL) {
        return NULL;
    }

    stack_level_t* newElement = (stack_level_t*)malloc(sizeof(stack_level_t));
    if(newElement == NULL) {
        return NULL;
    }
    newElement->next = NULL;
    newElement->table = symtable_init();
    if(newElement->table == NULL) {
        free(newElement);
        return NULL;
    }

    if(stack->top == NULL) {
        stack->top = newElement;
        stack->top->prev = NULL;
    }
    else {
        stack_level_t* current = stack->top;

        stack->top = newElement;
        stack->top->prev = current;
        current->next = newElement;
    }

    return stack->top;

}

htab_data_t* symtable_stack_search(symtable_stack_t* stack, char* key){
    if (stack == NULL || key == NULL) {
        return NULL;
    }

    stack_level_t* current = stack->top;

    while (current != NULL) {
        
        htab_data_t* result = symtable_search(current->table, key);
        if (result != NULL) {
            return result;  // Key found in the current level of the stack.
        }

        current = current->prev;  // Move to the previous level in the stack.
    }

    return NULL;  // Key not found in any level of the stack.

}