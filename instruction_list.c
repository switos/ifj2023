/*
IFJ projekt 2023
xtiemn00: Vsevolod Tiemnohorov
xpetro27: Denys Petrovsyi
xbatur00: Illia Baturov
xshish02: Sviatoslav Shishnev
*/
#include "instruction_list.h"

void DLL_Init( DLList *list ) {
	list->firstElement = NULL;
	list->activeElement = NULL;
	list->lastElement = NULL;
}

void DLL_Dispose( DLList *list ) {
	if (list != NULL) {
		if (list->firstElement != NULL) {
			DLLElementPtr tmpElement = list->firstElement;
			while(tmpElement != NULL) { 
				DLLElementPtr tmpElementNext = tmpElement->nextElement;
				free_data_value(tmpElement->data);
				free(tmpElement);
				tmpElement = tmpElementNext;
			}
			list->firstElement = NULL;
			list->activeElement = NULL;
			list->lastElement = NULL;
		}
	}
}


void DLL_InsertFirst( DLList *list, taCode* data ) {
	DLLElementPtr newElement  = (DLLElementPtr)malloc(sizeof(struct DLLElement)); 
	if (newElement == NULL) {
		return;
    }
	if (list->firstElement == NULL && list->lastElement == NULL) {
		newElement->data = data;
		newElement->nextElement = NULL;
		newElement->previousElement = NULL;
		list->firstElement = newElement;
		list->lastElement = newElement;
	}
	else { 
		newElement->data = data;
		list->firstElement->previousElement = newElement;
		newElement->nextElement = list->firstElement;
		newElement->previousElement = NULL;
		list->firstElement = newElement;
	}
}


void DLL_InsertLast( DLList *list, taCode* data ) {
	DLLElementPtr newElement  = (DLLElementPtr)malloc(sizeof(struct DLLElement)); 
	if (newElement == NULL) {
		return;
    }
	if (list->firstElement == NULL && list->lastElement == NULL) {
		newElement->data = data;
		newElement->nextElement = NULL;
		newElement->previousElement = NULL;
		list->lastElement = newElement;
		list->firstElement = newElement;
	}
	else { 
		list->lastElement->nextElement = newElement;
		newElement->data = data;
		newElement->nextElement = NULL;
		newElement->previousElement = list->lastElement;
		list->lastElement = newElement;
	}
}


void DLL_First( DLList *list ) {
	list->activeElement = list->firstElement;
}


void DLL_Last( DLList *list ) {
	list->activeElement = list->lastElement;
}


void DLL_GetFirst( DLList *list, taCode *dataPtr ) {
	if(list->firstElement == NULL) {
		return;
	}
	else {
		dataPtr = list->firstElement->data;
	}
}

void DLL_GetLast( DLList *list, taCode *dataPtr ) {
	if(list->firstElement == NULL) {
		return;
	}
	else {
		dataPtr = list->lastElement->data;
	}
}

void DLL_DeleteFirst( DLList *list ) {
	if (list->firstElement != NULL) {
		DLLElementPtr tmpElement = list->firstElement;
		if (list->activeElement == list->firstElement) {
			list->activeElement = NULL;
		}
		if (tmpElement->nextElement == NULL) {
			list->firstElement = NULL;
			list->lastElement = NULL;
		}
		else {
			list->firstElement = tmpElement->nextElement;
			list->firstElement->previousElement = NULL; 
		}
		free_data_value(tmpElement->data);
		free(tmpElement);
	}
}

void DLL_DeleteLast( DLList *list ) {
	if (list->firstElement != NULL) {
		DLLElementPtr tmpElement = list->lastElement;
		if (list->activeElement == list->lastElement) {
			list->activeElement = NULL;
		}
		if (tmpElement->previousElement == NULL){
			list->firstElement = NULL;
			list->lastElement = NULL;
		}
		else {
			list->lastElement = tmpElement->previousElement;
			list->lastElement->nextElement = NULL;
		}
		free_data_value(tmpElement->data);
		free(tmpElement);
	}
}

void DLL_DeleteAfter( DLList *list ) {
	if (list->activeElement != NULL && list->activeElement != list->lastElement) {
		DLLElementPtr tmpElement = list->activeElement->nextElement;
		if (tmpElement->nextElement != NULL) {
			list->activeElement->nextElement = tmpElement->nextElement;
			list->activeElement->nextElement->previousElement = list->activeElement;
		} else {
			list->activeElement->nextElement = NULL;
			list->lastElement = list->activeElement;
		}
		free_data_value(tmpElement->data);
		free(tmpElement);
	}
}

void DLL_DeleteBefore( DLList *list ) {
	if (list->activeElement != NULL && list->activeElement != list->firstElement) {
		DLLElementPtr tmpElement = list->activeElement->previousElement;
		if (tmpElement->previousElement != NULL) {
			list->activeElement->previousElement = tmpElement->previousElement;
			list->activeElement->previousElement->nextElement = list->activeElement;
		} else {
			list->activeElement->previousElement = NULL;
			list->firstElement = list->activeElement;
		}
		free_data_value(tmpElement->data);
		free(tmpElement);
	}
}

void DLL_InsertAfter( DLList *list, taCode* data ) {
	if (list->activeElement != NULL) {
		DLLElementPtr newElement  = (DLLElementPtr)malloc(sizeof(struct DLLElement));
		if (newElement == NULL) {
			return;
		} else {
			if (list->activeElement->nextElement != NULL) {
				newElement->nextElement = list->activeElement->nextElement;
				newElement->nextElement->previousElement = newElement;
			} else {
				newElement->nextElement = NULL;
				list->lastElement = newElement;
			}
			newElement->data = data;
			newElement->previousElement = list->activeElement;
			list->activeElement->nextElement = newElement;
		}
	}
}

void DLL_InsertBefore( DLList *list, taCode* data ) {
	if (list->activeElement != NULL) {
		DLLElementPtr newElement  = (DLLElementPtr)malloc(sizeof(struct DLLElement));
		if (newElement == NULL) {
			return;
		} else {
			if (list->activeElement->previousElement != NULL) {
				newElement->previousElement = list->activeElement->previousElement;
				newElement->previousElement->nextElement = newElement;
			} else {
				newElement->previousElement = NULL;
				list->firstElement = newElement;
			}
			newElement->data = data;
			newElement->nextElement = list->activeElement;
			list->activeElement->previousElement = newElement;
		}
	}
}

void DLL_GetValue( DLList *list, taCode *dataPtr ) {
	if (list->activeElement == NULL) {
		return;
	} else {
		dataPtr = list->activeElement->data;
	}
}

void DLL_SetValue( DLList *list, taCode* data ) {
	if (list->activeElement != NULL) {
		if (!insert_data(list->activeElement->data, data)) {
			DLL_Dispose(list);
			return;
		}
	}
}

void DLL_Next( DLList *list ) {
	if (list->activeElement != NULL) {
		list->activeElement = list->activeElement->nextElement;
	}
}

void DLL_Previous( DLList *list ) {
	if (list->activeElement != NULL) {
		list->activeElement = list->activeElement->previousElement;
	}
}

int DLL_IsActive( DLList *list ) {
	return (list->activeElement != NULL);
}

bool set_operand_value(operand_t* target, char* source) {
	if (target == NULL) {
		return false;
	}

	if (source == NULL) {
		target->value = NULL;
		return true;
	}

	target->value = (char*) malloc((strlen(source)+1) * __CHAR_BIT__);
	strncpy(target->value, source, strlen(source) + 1);
	return true;
}

void free_data_value(taCode* target) {
	if (target == NULL) {
		return;
	}

	if (target->operand_1->value != NULL) {
		free(target->operand_1->value);
	}
	target->operand_1->value = NULL;

	if (target->operand_2->value != NULL) {
		free(target->operand_2->value);
	}
	target->operand_2->value = NULL;

	if (target->result->value != NULL) {
		free(target->result->value);
	}
	target->result->value = NULL;

	if (target->operand_1->out)
		str_free(target->operand_1->out);

	if (target->operand_2->out)
		str_free(target->operand_2->out);

	if (target->result->out)
		str_free(target->result->out);

	free(target->operand_1->out);
	free(target->operand_2->out);
	free(target->result->out);

	free(target->operand_1);
	free(target->operand_2);
	free(target->result);

	free(target);
}

bool insert_data(taCode* target, taCode* source) {
	*target = *source;
	
	if (!set_operand_value(target->operand_1, source->operand_1->value)) {
		free_data_value(target);
		return false;
	}
	if (!set_operand_value(target->operand_2, source->operand_2->value)) {
		free_data_value(target);
		return false;
	}
	if (!set_operand_value(target->result, source->result->value)) {
		free_data_value(target);
		return false;
	}

	return true;
}

void clear_data(taCode* source) {
	if (source == NULL) {
 		return;
 	}

	source->result->frame = F_DEFAULT;
	source->result->type = ET_UNDEFINED;


	source->operand_1->frame = F_DEFAULT;
	source->operand_1->type = ET_UNDEFINED;


	source->operand_2->frame = F_DEFAULT;
	source->operand_2->type = ET_UNDEFINED;
	
	free_data_value(source);
	source->inst = I_DEFAULT;
}

operand_t* create_operand(char* value, ET_TYPE type, frame_type frame) {
	operand_t* new_operand = (operand_t*)malloc(sizeof(operand_t));
	if(new_operand == NULL) {
		fprintf(stderr, "Failed to allocate memory");
		return NULL;
	}

	set_operand_value(new_operand, value);
	new_operand->type = type;
	new_operand->frame = frame;
	return new_operand;
}

taCode* init_data() {
	taCode* newData = (taCode*)malloc(sizeof(taCode));

	newData->inst = I_DEFAULT;
	newData->operand_1 = (operand_t*)malloc(sizeof(operand_t));
	newData->operand_2 = (operand_t*)malloc(sizeof(operand_t));
	newData->result = (operand_t*)malloc(sizeof(operand_t));

	newData->operand_1->frame = F_DEFAULT;
	newData->operand_1->type = ET_UNDEFINED;
	newData->operand_1->value = NULL;
	newData->operand_1->out = (string*)malloc(sizeof(string));
	// newData->operand_1->out = NULL;
	// if (!str_init(newData->operand_1->out))
	// 	return NULL;

	newData->operand_2->frame = F_DEFAULT;
	newData->operand_2->type = ET_UNDEFINED;
	newData->operand_2->value = NULL;
	newData->operand_2->out = (string*)malloc(sizeof(string));
	// newData->operand_1->out = NULL;
	// if (!str_init(newData->operand_2->out))
	// 	return NULL;

	newData->result->frame = F_DEFAULT;
	newData->result->type = ET_UNDEFINED;
	newData->result->value = NULL;
	newData->result->out = (string*)malloc(sizeof(string));
	// newData->operand_1->out = NULL;
	// if (!str_init(newData->result->out))
	// 	return NULL;

	return newData;
}