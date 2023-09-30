#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "str.h"

// constant for memory allocation and reallocation
#define STR_LEN_INC 8

bool str_init(string* s) {
    if ((s->str = (char *) malloc(STR_LEN_INC)) == NULL) {
        return false;
    }
    s->allocSize = STR_LEN_INC;
    s->length = 0;
    s->str[0] = '\0';
    return true;
}

void str_free(string* s) {
    free(s->str);
}

void str_clear(string* s) {
    s->length = 0;
    s->str[0] = '\0';
}

bool str_add_char(string* s, char c) {
    if (s->length + 1 >= s->allocSize) { // this means we need more memory
    // printf("PIZDA TUT\n");
        if ((s->str = (char *) realloc(s->str, s->length + STR_LEN_INC)) == NULL) {
            return false;
        }
        s->allocSize = s->length + STR_LEN_INC;
    }
    s->str[s->length++] = c;
    s->str[s->length] = '\0';
    return true;
}

bool str_add_more_chars(string* target, char* str) {
    for (int i = 0; i < strlen(str); i++) {
        if (!str_add_char(target, str[i])) {
            return false;
        }
    }
    return true;
}

bool str_copy_string(string* target, string* source) {
    // printf("s.l : %d\ts.as : %d\ts.s : %s\n", source->length, source->allocSize, source->str);
    // printf("t.l : %d\tt.as : %d\tt.s : %s\n", target->length, target->allocSize, target->str);
    // printf("t.l : %d\tt.as : %d\tt.s : %s\n", target->length, target->allocSize, target->str);
    // if (target->allocSize == 0) {
    //     printf("NULLLLLL SUKAAAA\n");
    // }
    if (source->length >= target->allocSize) { // this means we need more memory
        if ((target->str = (char *) realloc(target->str, source->length + 1)) == NULL) {
            return false;
        }
        target->allocSize = source->length + 1;
    }
    strcpy(target->str, source->str);
    target->length = source->length;
    return true;
}

bool str_copy_const_string(char* target, string* source) {
    if (target == NULL || source == NULL) {
        return false;
    }

    target = (char*) malloc((source->length+1) * __CHAR_BIT__);
    if (target == NULL) {
        return false;
    }

    strncpy(target, source->str, source->length + 1);
    return true;
}

int str_cmp_string(string* s1, string* s2) {
    return strcmp(s1->str, s2->str);
}

int str_cmp_const_str(string* s1, char* s2) {
    return strcmp(s1->str, s2);
}

bool str_add_fun_name(string* target, char* name) {
    if (target->length == 0) {
        if (!str_add_char(target, '-')) {
            return false;
        }
    }

    for (int i = 0; i < strlen(name); i++) {
        if (!str_add_char(target, name[i])) {
            return false;
        }
    }

    if (!str_add_char(target, '-')) {
        return false;
    }
    return true;
}

bool str_find_fun_name(string* target, char* name) {
    bool start_flag = false;
    string tmp;
    if (!str_init(&tmp)) {
        return false;
    }

    for (int i = 0; i < target->length; i++) {
        if (!start_flag && target->str[i] == '-') {
            start_flag = true;
        } else if (start_flag && target->str[i] != '-') {
            if (!str_add_char(&tmp, target->str[i])) {
                str_free(&tmp);
                return false;
            }
        } else {
            if (!str_cmp_const_str(&tmp, name)) {
                return true;
            } else {
                str_clear(&tmp);
            }
        }
    }

    str_free(&tmp);
    return false;
}

void str_get_last_fun_name(string* source, char** target) {
    if (source->length == 0) {
        return;
    }

    string tmp;
    int i = source->length-1;
    if (!str_init(&tmp)) {
        return;
    }

    while (source->str[--i] != '-') {
    }

    while (++i < source->length-1) {
        if (!str_add_char(&tmp, source->str[i])) {
            str_free(&tmp);
            return;
        }
    }

    *target = (char *) malloc((tmp.length + 1) * __CHAR_BIT__);
    if (*target == NULL) {
        str_free(&tmp);
        return;
    }
    
    strncpy(*target, tmp.str, tmp.length + 1);

    str_free(&tmp);
}