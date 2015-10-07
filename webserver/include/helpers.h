#ifndef HELPERS_H
#define HELPERS_H

typedef struct Configuration {
    char* dir;
    int port;
    char* index;
    char* logfile;
    char* requestHandlingMethod;
} Configuration;

/**
 * Reads a path and returns the content of it
 *
 * @param  pathToFile filePaht
 * @return            content of the given filePath
 */
char* readFromFile(char* pathToFile);


void readConfiguration(Configuration *config);

#endif
