#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>

#include "helpers.h"
#include "logging.h"

char* readFromFile(char* pathToFile) {
    char* content = (char *)malloc(sizeof(char) * 4096);
    int fd;
    struct stat fileStat;

    fd = open(pathToFile, O_RDWR);

    if (fd == -1) {
        printf("readFromFile: FAILED TO OPEN FILE: %s\n", pathToFile);
        return NULL;
    }

    if (fstat(fd, &fileStat) == -1) {
        log_fail("readFromFile: Failed to get stats on file.\n");
        exit(1);
    }

    if ((content = (char *) mmap(0, fileStat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED) {
        log_fail("readFromFile: Failed to mmap file\n");
        exit(1);
    }

    //close(pathToFile);

    return content;
}

void readConfiguration(Configuration* config, char* configFileName) {
    char* content = readFromFile(configFileName);

    if (content == NULL) {
        printf("Config file not found.\n");
        exit(3);
    }

    char* temp = malloc(strlen(content) + 1);
    strncpy(temp, content, strlen(content));
    char* token;

    // Step 1: DIR
    token = strtok(temp, "=");
    token = strtok(NULL, "\n");
    config->dir = token;

    // Step 2: PORT
    token = strtok(NULL, "=");
    token = strtok(NULL, "\n");
    config->port = atoi(token);

    // Step 3 Index
    token = strtok(NULL, "=");
    token = strtok(NULL, "\n");
    config->index = token;

    // Step 4 LOG:
    token = strtok(NULL, "=");
    token = strtok(NULL, "\n");
    config->logfile = token;

    // Step 5 Method
    token = strtok(NULL, "=");
    token = strtok(NULL, "\n");
    config->requestHandlingMethod = token;
}
