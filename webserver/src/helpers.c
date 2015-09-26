#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "helpers.h"

void concateStr(const char* str1, const char* str2, char* result) {
    // char* strToReturn;

    // should we use realloc instead and just realloc the str1
    result = (char*) malloc(strlen(str1) + strlen(str2));
    strcpy(result, str1);
    strcat(result, str2);

    // return strToReturn;
}
