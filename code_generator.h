#ifndef _CODE_GENERATOR_H
#define _CODE_GENERATOR_H

#include <stdio.h>
#include <stdbool.h>

#include "scanner.h"
#include "symtable.h"
#include "str.h"
#include "instruction_list.h"

void print_instruction(DLList* list);

#endif