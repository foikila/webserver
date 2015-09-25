#include <stdlib.h>
#include <stdio.h>

#include "logging.h"

int log_fail(char* msg) {
    perror(msg);
    // TODO fix log to file
    return 0;
}

int log_success(char* msg) {
    printf("SUCCESS: %s\n", msg);
    return 0;
}
