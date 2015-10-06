#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>

#include "helpers.h"

char* readFromFile(char* pathToFile) {
    char* content = (char *)malloc(sizeof(char) * 4096);
    int fd;
    struct stat fileStat;

    fd = open(pathToFile, O_RDWR);

    if (fd == -1) {
        //log_fail("READFROMFILE: Failed to open file");
        printf("READFROMFILE: FAILED TO OPEN FILE");
        return NULL;
    }

    if (fstat(fd, &fileStat) == -1) {
        log_fail("READFROMFILE: Failed to get stats on file.");
        exit(1);
    }

    if ((content = (char *) mmap(0, fileStat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED) {
        log_fail("READFROMFILE: Failed to mmap file");
        exit(1);
    }

    return content;
}
