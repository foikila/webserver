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

    //close(pathToFile);

    return content;
}

void readConfiguration(Configuration *config) {
    /*char* filename = "config.conf";
    char* content = readFromFile(filename);

    char* temp = malloc(strlen(content) + 1);
    strncpy(temp, content, strlen(content));

    printf("\n%s\n\n", temp);*/
    /*char *dir, *index, *log, *method;
    int port;

    char* split;*/
    //char *name;
    /*char* point;
    point = strtok(temp, "=");
    int i = 0;
    while (point != NULL) {

        printf("%d \t", i);

        if (i == 1) {
            config->dir = point;
        } else if (i == 2) {
            config->port = atoi(point);
        } else if (i == 3) {
            config->index = point;
        } else if (i == 4) {
            config->logfile = point;
        } else if (i == 5) {
            config->requestHandlingMethod = point;
        }

        printf("%s\n", point);
        point = strtok(NULL, "=");
        //point = strtok(NULL, "\n");
        i++;
    }*/

    config->dir = "/home/olund/unix/webserver";
    config->port = 1337;
    config->logfile = "/var/log/test";
    config->requestHandlingMethod = "FORK";
    config->index = "/index.html";
}
