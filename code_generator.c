#include "code_generator.h"

void codeStart() {
	printf(".IFJcode23\n\
	DEFVAR GF@tmp1\n\
	DEFVAR GF@tmp2\n\
	DEFVAR GF@tmp3\n\
	JUMP $$main\n\
	\n\
	LABEL $readi\n\
		PUSHFRAME\n\
		DEFVAR LF@VAR\n\
		READ LF@VAR string@int\n\
		POPFRAME\n\
		RETURN\n\
	\n\
	LABEL $readf\n\
		PUSHFRAME\n\
		DEFVAR LF@VAR\n\
		READ LF@VAR string@float\n\
		POPFRAME\n\
		RETURN\n\
	\n\
	LABEL $reads\n\
		PUSHFRAME\n\
		DEFVAR LF@VAR\n\
		READ LF@VAR string@string\n\
		POPFRAME\n\
		RETURN\n\
	\n\
	LABEL $floatval\n\
		PUSHFRAME\n\
		DEFVAR LF@$floatval$return\n\
		MOVE LF@$floatval$return LF@%%0\n\
		TYPE GF@tmp1 LF@%%0\n\
		JUMPIFEQ $floatval$type$int GF@tmp1 string@int\n\
		JUMPIFEQ $floatval$type$nil GF@tmp1 string@nil\n\
		JUMP $floatval$end\n\
		LABEL $floatval$type$int\n\
		INT2FLOAT LF@$floatval$return LF@%%0\n\
		JUMP $floatval$end\n\
		LABEL $floatval$type$nil\n\
		INT2FLOAT LF@$floatval$return int@0\n\
		LABEL $floatval$end\n\
		POPFRAME\n\
		RETURN\n\
	\n\
	LABEL $intval\n\
		PUSHFRAME\n\
		DEFVAR LF@$intval$return\n\
		MOVE LF@$intval$return LF@%%0\n\
		TYPE GF@tmp1 LF@%%0\n\
		JUMPIFEQ $intval$type$float GF@tmp1 string@float\n\
		JUMPIFEQ $intval$type$nil GF@tmp1 string@nil\n\
		JUMP $intval$end\n\
		LABEL $intval$type$float\n\
		FLOAT2INT LF@$intval$return LF@%%0\n\
		JUMP $intval$end\n\
		LABEL $intval$type$nil\n\
		MOVE LF@$intval$return int@0\n\
		LABEL $intval$end\n\
		POPFRAME\n\
		RETURN\n\
	\n\
	LABEL $strval\n\
		PUSHFRAME\n\
		DEFVAR LF@$strval$return\n\
		MOVE LF@$strval$return LF@%%0\n\
		TYPE GF@tmp1 LF@%%0\n\
		JUMPIFEQ $strval$type$nil GF@tmp1 string@nil\n\
		JUMP $strval$end\n\
		LABEL $strval$type$nil\n\
		MOVE LF@$strval$return string@ \n\
		LABEL $strval$end\n\
		POPFRAME\n\
		RETURN\n\
	\n\
	LABEL $strlen\n\
		PUSHFRAME\n\
		DEFVAR LF@$strlen$return\n\
		STRLEN LF@$strlen$return LF@%%0\n\
		POPFRAME\n\
		RETURN\n\
	\n\
	LABEL $substring\n\
		PUSHFRAME\n\
		DEFVAR LF@$substring$return\n\
		MOVE LF@$substring$return string@ \n\
		DEFVAR LF@$substring$control\n\
		LT LF@$substring$control LF@%%1 int@0\n\
		JUMPIFEQ $substring$end LF@$substring$control bool@true\n\
		LT LF@$substring$control LF@%%2 int@0\n\
		JUMPIFEQ $substring$end LF@$substring$control bool@true\n\
		GT LF@$substring$control LF@%%1 LF@%%2\n\
		JUMPIFEQ $substring$end LF@$substring$control bool@true\n\
		DEFVAR LF@$substring$len\n\
		STRLEN LF@$substring$len LF@%%0\n\
		GT LF@$substring$control LF@%%1 LF@$substring$len\n\
		JUMPIFEQ $substring$end LF@$substring$control bool@true\n\
		GT LF@$substring$control LF@%%2 LF@$substring$len\n\
		JUMPIFEQ $substring$end LF@$substring$control bool@true\n\
		DEFVAR LF@$substring$char\n\
		LABEL $substring$loop\n\
		GETCHAR LF@$substring$char LF@%%0 LF@%%1\n\
		CONCAT LF@$substring$return LF@$substring$char\n\
		LT LF@$substring$control LF@%%1 LF@%%2\n\
		JUMPIFEQ $substring$loop LF@$substring$control bool@true\n\
		LABEL $substring$end\n\
		POPFRAME\n\
		RETURN\n\
	\n\
	LABEL $ord\n\
		PUSHFRAME\n\
		DEFVAR LF@$ord$return\n\
		MOVE LF@$ord$return int@0\n\
		DEFVAR LF@$ord$len\n\
		STRLEN LF@$ord$len LF%%0\n\
		JUMPIFEQ $ord$end LF@$ord$len int@0\n\
		STRI2INT LF@$ord$return LF%%0 int@0\n\
		LABLE $ord$end\n\
		POPFRAME\n\
		RETURN\n\
	\n\
	LABLE $chr\n\
		PUSHFRAME\n\
		DEFVAR LF@$chr$return\n\
		INT2CHAR LF@$chr$return LF@%%0\n\
		POPFRAME\n\
		RETURN\n\
	\n\
	LABLE $write\n\
		PUSHFRAME\n\
		DEFVAR LF@$write$var\n\
		LABEL $write$loop\n\
		POPS LF@$write$var\n\
		JUMPIFEQ $write$end LF@$write$var nil@nil\n\
		WRITE LF@$write$var\n\
		JUMP $write$loop\n\
		LABLE $write$end\n\
		POPFRAME\n\
		RETURN\n\
	");
}

bool pregenerating_instructions(taCode* data) {
	if (!str_init(&data->result->out)) {
		return false;
	}
	
// Хохол, прочитав плакат "Бей жидов, спасай Россию":
// - Лозунг гарный... А циль погана...

	if (!str_init(&data->operand_1->out)) {
		return false;
	}

	if (!str_init(&data->operand_2->out)) {
		return false;
	}

	if (data->result->value != NULL) {

		if (data->result->frame == F_GF) {
			str_add_more_chars(&data->result->out, "GF@");
			str_add_more_chars(&data->result->out, data->result->value);
		} else if (data->result->frame == F_LF) {
			str_add_more_chars(&data->result->out, "LF@");
			str_add_more_chars(&data->result->out, data->result->value);
		} else if (data->result->frame == F_TF) {
			str_add_more_chars(&data->result->out, "TF@");
			str_add_more_chars(&data->result->out, data->result->value);
		} else if (data->result->type == ET_INT || data->result->type == ET_INTN) {
			str_add_more_chars(&data->result->out, "int@");
			str_add_more_chars(&data->result->out, data->result->value);
		} else if (data->result->type == ET_DOUBLE || data->result->type == ET_DOUBLEN) {
			str_add_more_chars(&data->result->out, "float@");
			str_add_more_chars(&data->result->out, data->result->value);
			int i = strlen(data->result->value);
			char c[i+1];
			double d = atof(data->result->value);
			sprintf(c, "%a", d);
			str_add_more_chars(&data->result->out, c);
		} else if (data->result->type == ET_STRING || data->result->type == ET_STRINGN) {
			str_add_more_chars(&data->result->out, "string@");
			str_add_more_chars(&data->result->out, data->result->value);
		} else if (data->result->type == ET_NIL) {
			str_add_more_chars(&data->result->out, "nil@");
			str_add_more_chars(&data->result->out, data->result->value);
		} else {
			str_add_more_chars(&data->result->out, data->result->value);
		}
		

	} 
	
	if (data->operand_1->value != NULL) {

		if (data->operand_1->frame == F_GF) {
			str_add_more_chars(&data->operand_1->out, "GF@");
			str_add_more_chars(&data->operand_1->out, data->operand_1->value);
		} else if (data->operand_1->frame == F_LF) {
			str_add_more_chars(&data->operand_1->out, "LF@");
			str_add_more_chars(&data->operand_1->out, data->operand_1->value);
		} else if (data->operand_1->frame == F_TF) {
			str_add_more_chars(&data->operand_1->out, "TF@");
			str_add_more_chars(&data->operand_1->out, data->operand_1->value);
		} else if (data->result->type == ET_INT || data->result->type == ET_INTN) {
			str_add_more_chars(&data->operand_1->out, "int@");
			str_add_more_chars(&data->operand_1->out, data->operand_1->value);
		} else if (data->operand_1->type == ET_DOUBLE) {
			str_add_more_chars(&data->operand_1->out, "float@");
			int i = strlen(data->operand_1->value);
			char c[i+1];
			double d = atof(data->operand_1->value);
			sprintf(c, "%a", d);
			str_add_more_chars(&data->operand_1->out, c);
		} else if (data->result->type == ET_STRING || data->result->type == ET_STRINGN) {
			str_add_more_chars(&data->operand_1->out, "string@");
			str_add_more_chars(&data->operand_1->out, data->operand_1->value);
		} else if (data->operand_1->type == ET_NIL) {
			str_add_more_chars(&data->operand_1->out, "nil@");
			str_add_more_chars(&data->operand_1->out, data->operand_1->value);
		} else {
			str_add_more_chars(&data->operand_1->out, data->operand_1->value);
		}

	} 

	if(data->operand_2->value != NULL) {

		if (data->operand_2->frame == F_GF) {
			str_add_more_chars(&data->operand_2->out, "GF@");
			str_add_more_chars(&data->operand_2->out, data->operand_2->value);
		} else if (data->operand_2->frame == F_LF) {
			str_add_more_chars(&data->operand_2->out, "LF@");
			str_add_more_chars(&data->operand_2->out, data->operand_2->value);
		} else if (data->operand_2->frame == F_TF) {
			str_add_more_chars(&data->operand_2->out, "TF@");
			str_add_more_chars(&data->operand_2->out, data->operand_2->value);
		} else if (data->result->type == ET_INT || data->result->type == ET_INTN) {
			str_add_more_chars(&data->operand_2->out, "int@");
			str_add_more_chars(&data->operand_2->out, data->operand_2->value);
		} else if (data->operand_2->type == ET_DOUBLE) {
			str_add_more_chars(&data->operand_2->out, "float@");
			int i = strlen(data->operand_2->value);
			char c[i+1];
			double d = atof(data->operand_2->value);
			sprintf(c, "%a", d);
			str_add_more_chars(&data->operand_2->out, c);
		} else if (data->result->type == ET_STRING || data->result->type == ET_STRINGN) {
			str_add_more_chars(&data->operand_2->out, "string@");
			str_add_more_chars(&data->operand_2->out, data->operand_2->value);
		} else if (data->operand_2->type == ET_NIL) {
			str_add_more_chars(&data->operand_2->out, "nil@");
			str_add_more_chars(&data->operand_2->out, data->operand_2->value);
		} else {
			str_add_more_chars(&data->operand_2->out, data->operand_2->value);
		}
	} 
	return true;
}

void print_instruction(DLList* list) {
	codeStart();
	DLL_First(list);
	while (DLL_IsActive(list)) {
		pregenerating_instructions(&list->activeElement->data);
		switch (list->activeElement->data->inst){
			case I_COMMENT:
				printf("# %s\n", list->activeElement->data->operand_1->out.str);
				break;

			case I_MOVE:
				printf("MOVE %s %s\n", list->activeElement->data->result->out.str, list->activeElement->data->operand_1->out.str);
				break;
			case I_CREATEFRAME:
				printf("CREATEFRAME\n");
				break;
			case I_PUSHFRAME:
				printf("PUSHFRAME\n");
				break;
			case I_POPFRAME:
				printf("POPFRAME\n");
				break;
			case I_DEFVAR:
				printf("DEFVAR %s\n", list->activeElement->data->operand_1->out.str);
				break;
			case I_CALL:
				printf("CALL %s\n", list->activeElement->data->operand_1->out.str);
				break;
			case I_RETURN:
				printf("RETURN %s\n", list->activeElement->data->operand_1->out.str);
				break;


			case I_PUSHS:
				printf("PUSHS\n");
				break;
			case I_POPS:
				printf("POPS\n");
				break;
			case I_CLEARS:
				printf("CLEARS\n");
				break;

			
			case I_ADD:
				printf("ADD %s %s %s\n", list->activeElement->data->result->out.str, list->activeElement->data->operand_1->out.str, list->activeElement->data->operand_2->out.str);
				break;
			case I_SUB:
				printf("SUB %s %s %s\n", list->activeElement->data->result->out.str, list->activeElement->data->operand_1->out.str, list->activeElement->data->operand_2->out.str);
				break;
			case I_MUL:
				printf("MUL %s %s %s\n", list->activeElement->data->result->out.str, list->activeElement->data->operand_1->out.str, list->activeElement->data->operand_2->out.str);
				break;
			case I_DIV:
				printf("DIV %s %s %s\n", list->activeElement->data->result->out.str, list->activeElement->data->operand_1->out.str, list->activeElement->data->operand_2->out.str);
				break;
			case I_IDIV:
				printf("IDIV %s %s %s\n", list->activeElement->data->result->out.str, list->activeElement->data->operand_1->out.str, list->activeElement->data->operand_2->out.str);
				break;

			
			case I_ADDS:
				printf("ADDS\n");
				break;
			case I_SUBS:
				printf("SUBS\n");
				break;
			case I_MULS:
				printf("MULS\n");
				break;
			case I_DIVS:
				printf("DIVS\n");
				break;
			case I_IDIVS:
				printf("IDIVS\n");
				break;


			case I_LT:
				printf("LT %s %s %s\n", list->activeElement->data->result->out.str, list->activeElement->data->operand_1->out.str, list->activeElement->data->operand_2->out.str);
				break;
			case I_GT:
				printf("GT %s %s %s\n", list->activeElement->data->result->out.str, list->activeElement->data->operand_1->out.str, list->activeElement->data->operand_2->out.str);
				break;
			case I_EQ:
				printf("EQ %s %s %s\n", list->activeElement->data->result->out.str, list->activeElement->data->operand_1->out.str, list->activeElement->data->operand_2->out.str);
				break;
			case I_LTS:
				printf("LTS\n");
				break;
			case I_GTS:
				printf("GTS\n");
				break;
			case I_EQS:
				printf("EQS\n");
				break;


			case I_AND:
				printf("AND %s %s %s\n", list->activeElement->data->result->out.str, list->activeElement->data->operand_1->out.str, list->activeElement->data->operand_2->out.str);
				break;
			case I_OR:
				printf("OR %s %s %s\n", list->activeElement->data->result->out.str, list->activeElement->data->operand_1->out.str, list->activeElement->data->operand_2->out.str);
				break;
			case I_NOT:
				printf("NOT %s %s %s\n", list->activeElement->data->result->out.str, list->activeElement->data->operand_1->out.str, list->activeElement->data->operand_2->out.str);
				break;
			case I_ANDS:
				printf("ANDS\n");
				break;
			case I_ORS:
				printf("ORS\n");
				break;
			case I_NOTS:
				printf("NOTS\n");
				break;


			case I_INT2FLOAT:
				printf("INT2FLOAT %s %s\n", list->activeElement->data->result->out.str, list->activeElement->data->operand_1->out.str);
				break;
			case I_FLOAT2INT:
				printf("FLOAT2INT %s %s\n", list->activeElement->data->result->out.str, list->activeElement->data->operand_1->out.str);
				break;
			case I_INT2CHAR:
				printf("INT2CHAR %s %s\n", list->activeElement->data->result->out.str, list->activeElement->data->operand_1->out.str);
				break;
			case I_STRI2INT:
				printf("STRI2INT %s %s\n", list->activeElement->data->result->out.str, list->activeElement->data->operand_1->out.str);
				break;


			case I_INT2FLOATS:
				printf("INT2FLOATS\n");
				break;
			case I_FLOAT2INTS:
				printf("FLOAT2INTS\n");
				break;
			case I_INT2CHARS:
				printf("INT2CHARS\n");
				break;
			case I_STRI2INTS:
				printf("STRI2INTS\n");
				break;


			case I_READ:
				printf("READ %s %s\n", list->activeElement->data->result->out.str, list->activeElement->data->operand_1->out.str);
				break;
			case I_WRITE:
				printf("WRITE %s\n", list->activeElement->data->operand_1->out.str);
				break;
			

			case I_CONCAT:
				printf("CONCAT %s %s %s\n", list->activeElement->data->result->out.str, list->activeElement->data->operand_1->out.str, list->activeElement->data->operand_2->out.str);
				break;
			case I_STRLEN:
				printf("STRLEN %s %s\n", list->activeElement->data->result->out.str, list->activeElement->data->operand_1->out.str);
				break;
			case I_GETCHAR:
				printf("GETCHAR %s %s %s\n", list->activeElement->data->result->out.str, list->activeElement->data->operand_1->out.str, list->activeElement->data->operand_2->out.str);
				break;
			case I_SETCHAR:
				printf("SETCHAR %s %s %s\n", list->activeElement->data->result->out.str, list->activeElement->data->operand_1->out.str, list->activeElement->data->operand_2->out.str);
				break;

			
			case I_TYPE:
				printf("TYPE %s %s\n", list->activeElement->data->result->out.str, list->activeElement->data->operand_1->out.str);
				break;


			case I_LABEL:
				printf("LABEL %s\n", list->activeElement->data->operand_1->out.str);
				break;
			case I_JUMP:
				printf("JUMP %s\n", list->activeElement->data->operand_1->out.str);
				break;
			case I_JUMPIFEQ:
				printf("JUMPIFEQ %s %s %s\n", list->activeElement->data->result->out.str, list->activeElement->data->operand_1->out.str, list->activeElement->data->operand_2->out.str);
				break;
			case I_JUMPIFNEQ:
				printf("JUMPIFNEQ %s %s %s\n", list->activeElement->data->result->out.str, list->activeElement->data->operand_1->out.str, list->activeElement->data->operand_2->out.str);
				break;
			case I_JUMPIFEQS:
				printf("JUMPIFEQS %s\n", list->activeElement->data->operand_1->out.str);
				break;
			case I_JUMPIFNEQS:
				printf("JUMPIFNEQS %s\n", list->activeElement->data->operand_1->out.str);
				break;
			case I_EXIT:
				printf("EXIT %s\n", list->activeElement->data->operand_1->out.str);
				break;


			case I_BREAK:
				printf("BREAK\n");
				break;
			case I_DPRINT:
				printf("DPRINT %s\n", list->activeElement->data->operand_1->out.str);
				break;

			case I_DEFAULT:
				break;
		}

		// if (&list->activeElement->data->result->out != NULL) {
			str_free(&list->activeElement->data->result->out);
		// }
		// if (&list->activeElement->data->operand_1->out != NULL) {
			str_free(&list->activeElement->data->operand_1->out);
		// }
		// if (&list->activeElement->data->operand_2->out != NULL) {
			str_free(&list->activeElement->data->operand_2->out);
		// }
		DLL_Next(list);
	}
}

void output_main_func(DLList* list) {
	taCode* data = (taCode*)malloc(sizeof(taCode));
	if(data = NULL) {
		fprintf(stderr, "Failed to allocate memory");
		return;
	}

	data->operand_1 = (operand_t*)malloc(sizeof(operand_t));
	set_operand_value(data->operand_1, "$$main");
	data->inst = I_LABEL;
	
	DLL_InsertFirst(list, data);
	DLL_First(list);
}

void output_user_func_start(DLList* list, operand_t* op1) {
	taCode* data_label = (taCode*)malloc(sizeof(taCode));
	if(data_label = NULL) {
		fprintf(stderr, "Failed to allocate memory");
		return;
	}

	data_label->operand_1 = op1;
	data_label->inst = I_LABEL;
	DLL_InsertFirst(list, data_label);
	DLL_First(list);

	taCode* data_push = (taCode*)malloc(sizeof(taCode));
	if(data_push = NULL) {
		fprintf(stderr, "Failed to allocate memory");
		return;
	}
	data_push->inst = I_PUSHFRAME;
	DLL_InsertAfter(list, data_push);
	DLL_Next(list);
}

void output_user_func_return(DLList* list) {
	taCode* data_pop = (taCode*)malloc(sizeof(taCode));
	if(data_pop = NULL) {
		fprintf(stderr, "Failed to allocate memory");
		return;
	}

	data_pop->inst = I_POPFRAME;
	DLL_InsertAfter(list, data_pop);
	DLL_Next(list);

	taCode* data_return = (taCode*)malloc(sizeof(taCode));
	if(data_return = NULL) {
		fprintf(stderr, "Failed to allocate memory");
		return;
	}
	data_return->inst = I_RETURN;
	DLL_InsertAfter(list, data_return);
	DLL_Last(list);
}

void output_instruction_3args(DLList* list, operand_t* op1, operand_t* op2, operand_t* res, instructions inst) {
	taCode* data = (taCode*)malloc(sizeof(taCode));
	if(data = NULL) {
		fprintf(stderr, "Failed to allocate memory");
		return;
	}

	data->operand_1 = op1;
	data->operand_2 = op2;
	data->result = res;
	data->inst = inst;
	DLL_InsertAfter(list, data);
	DLL_Next(list);
}

void output_instruction_2args(DLList* list, operand_t* op1, operand_t* res, instructions inst) {
	taCode* data = (taCode*)malloc(sizeof(taCode));
	if(data = NULL) {
		fprintf(stderr, "Failed to allocate memory");
		return;
	}

	data->operand_1 = op1;
	data->result = res;
	data->inst = inst;
	DLL_InsertAfter(list, data);
	DLL_Next(list);
}

void output_instruction_1arg(DLList* list, operand_t* op1, instructions inst) {
	taCode* data = (taCode*)malloc(sizeof(taCode));
	if(data = NULL) {
		fprintf(stderr, "Failed to allocate memory");
		return;
	}

	data->operand_1 = op1;
	data->inst = inst;
	DLL_InsertAfter(list, data);
	DLL_Next(list);
}

void output_instruction_noargs(DLList* list, instructions inst) {
	taCode* data = (taCode*)malloc(sizeof(taCode));
	if(data = NULL) {
		fprintf(stderr, "Failed to allocate memory");
		return;
	}

	data->inst = inst;
	DLL_InsertAfter(list, data);
	DLL_Next(list);
}