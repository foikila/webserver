#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>

// Our includes
#include "statusCodes.h"
#include "logging.h"

// Definitions
#define BACKLOG 10
#define BUFF_SIZE 1024
#define DEBUG 1

typedef struct Response {
    char* header;
    char* body;
    int size;
} Response;

typedef struct Request {
    char* method;
    char* uri;
} Request;

/**
 * Binds the socket.
 * Than listen on the port
 * @return [description]
 *
 */
int bindListen();

char* readFromFile(char* pathToFile);

void buildRequest(Request *req, char* requestFromClient);

void buildResponse(Response *res, char* body, char* contentType);

int main(int argc, char *argv[]) {
    int port = 1337;
    // TODO config stuff

    // create socket
    int serverSocket, clientSocket;
    socklen_t addrlen;
    char* requestBuffer = malloc(BUFF_SIZE);
    struct sockaddr_in address;
    struct Request req;
    struct Response res;

    // ipv4
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // Creates the serverSocket
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        log_fail("Failed to bind create socket");
    } else {
        log_success("Creating server socket: success!");
    }

    // Binds the socket to the address
    if (bind(serverSocket, (struct sockaddr *) &address, sizeof(address)) == -1) {
        log_fail("Failed to bind server socket");
        exit(1);
    } else {
        log_success("Binding server socket: success!");
    }

    // listen
    while (1) {
        if (listen(serverSocket, BACKLOG) == -1) {
            log_fail("server failed to listen.");
            exit(1);
        } else {
            log_success("Started to listen");
        }

        addrlen = sizeof(address);
        if ((clientSocket = accept(serverSocket, (struct sockaddr *) &address, &addrlen)) == -1) {
            log_fail("server failed to accept.");
            exit(1);
        }

        if (clientSocket > 0) {
            log_success("A client connected!");
        }

        // Takes the request from the client and puts it in the requestBuffer.
        recv(clientSocket, requestBuffer, BUFF_SIZE, 0);
        log_success(requestBuffer);

        // builds the request struct with the request uri and method
        buildRequest((struct Request*) &req, requestBuffer);

        // "switches" on request method
        if (strcmp(req.method, "GET")) {
            char * requestFile = "index.html";
            // TODO pass in the request uri here instead of index.html.
            // But only if the URI is diffrent than / or index.html
            // ^ Not sure if solved ^
            if (!strcmp(req.uri, "/") || !strcmp(req.uri, "/index.html")) {
                requestFile = req.uri;
            }
            char*path = "www/";
            concateStr(&path, requestFile);
            printf("%s\n", path);
            path = "www/index.html";
            if (DEBUG) {
                printf("DEBUG: Requested file path: %s\n", path);
            }

            char* responseToClient = readFromFile(path);
            buildResponse((struct Response *) &res, responseToClient, "text/html");
        } else if(strcmp(req.method, "HEAD")) {
            // TODO just sent the header
        } else {
            // TODO send method not allowed
        }

        // TODO Here we should just join header and body and do one write()
        write(clientSocket, res.header, strlen(res.header));
        write(clientSocket, res.body, strlen(res.body));

        close(clientSocket);
    }
    close(serverSocket);
    free(requestBuffer);

    // TODO unbind serverSocket
    exit(0);
}

void buildRequest(Request *req, char* requestFromClient) {
    char* tooken, *firstLine;
    firstLine = strtok(requestFromClient, "\n");
    tooken = strtok(firstLine, " ");
    req->method = tooken;
    req->uri = strtok(NULL, " ");

    if (DEBUG) {
        printf("METHOD: %s\n", req->method);
        printf("URI: %s\n", req->uri);
    }
}


void buildResponse(Response *res, char* body, char* contentType) {
    const char* header = "HTTP/1.0 %s\n"
                         "Content-type: %s\n"
                         "content-length: %d\n"
                         "\n";

    int bodySize = strlen(body);
    int headerSize = strlen(header);

    res->body = (char*) malloc(bodySize);
    res->body = body;

    res->header = (char*) malloc(headerSize);
    res->size = bodySize + headerSize;
    // Copies the header to the response header with the parameters
    // OK, contentType and size of the response
    sprintf(res->header, header, OK, contentType, res->size);

    if (DEBUG) {
        printf("----\n");
        printf("HEADER: %s\n", res->header);
        printf("BODY: %s\n", res->body);
        printf("----\n");
    }
}


char* readFromFile(char* pathToFile) {
    char* content;
    int fd;
    struct stat fileStat;
    printf("%s\n", pathToFile);
    fd = open(pathToFile, O_RDWR);

    if (fd == -1) {
        log_fail("Failed to open file");
        exit(1);
    }

    if (fstat(fd, &fileStat) == -1) {
        log_fail("Failed to get stats on file.");
        exit(1);
    }

    if ((content = (char *) mmap(0, fileStat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED) {
        log_fail("Failed to mmap file");
        exit(1);
    }

    fprintf(stdout, "%s\n", content);

    return content;
}
