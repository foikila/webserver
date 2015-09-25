#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "helpers.h"

char* concateStr(const char* str1, const char* str2) {
    char* strToReturn;

    // should we use realloc instead and just realloc the str1??
    strToReturn = (char*) malloc(strlen(str1) + strlen(str2));
    strcpy(strToReturn, str1);
    strcat(strToReturn, str2);

    return strToReturn;
}
