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
#include "helpers.h"

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

void buildRequest(Request *req, char* requestFromClient);

void buildResponse(Response *res, char* body, char* contentType, char* responseCode);

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
        exit(1);
    } else {
        log_success("Creating server socket: success!");
    }

    // http://stackoverflow.com/questions/548879/releasing-bound-ports-on-process-exit
    int iSetOption = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&iSetOption,
        sizeof(iSetOption));

    // Binds the socket to the address
    if (bind(serverSocket, (struct sockaddr *) &address, sizeof(address)) == -1) {
        log_fail("Failed to bind server socket");
        exit(1);
    } else if (DEBUG) {
        log_success("Binding server socket: success!");
    }

    char* responseToClient = (char *) malloc(1024);

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
        if (strcmp(req.method, "GET") == 0) {
            // TODO Should be configurable
            const char* basePath = "www";
            // TODO not sure about the size of the requestFile.
            char* requestFile = malloc(sizeof(char*) * 30);

            // No request uri given. Give default file.
            if (strcmp(req.uri, "/") == 0) {
                // TODOccess(requestBuffer); defualt file should be configurable
                requestFile = "/index.html";
            } else {
                requestFile = req.uri;
            }

            char* fullPath = malloc(strlen(basePath) + strlen(requestFile) + 1);

            // Combinds the request file and the requestfile to one path
            snprintf(fullPath, strlen(basePath) + strlen(requestFile) + 1, "%s%s", basePath, requestFile);

            if (DEBUG) {
                printf("DEBUG: Requested file path: %s\n", fullPath);
            }

            // needs to alloc to char* size. If the read fails
            //responseToClient = (char *) malloc(sizeof(char*));
            responseToClient = readFromFile(fullPath);

            // File not found. Sending 404
            if (responseToClient == NULL) {
                responseToClient = (char *) malloc(sizeof(char*) * 24);
                responseToClient = "404 document not found.\n";
                buildResponse((struct Response *) &res, responseToClient, "text/plain", OK);
            } else {
                // everything is ok. Send the requested file and ok status code
                buildResponse((struct Response *) &res, responseToClient, "text/html", OK);
            }


            // TODO Here we should just join header and body and do one write()
            write(clientSocket, res.header, strlen(res.header));
            write(clientSocket, res.body, strlen(res.body));


            // free(responseToClient);
            // free(res.header);
            // free(res.body);

        } else if(strcmp(req.method, "HEAD") == 0) {
            buildResponse((struct Response *) &res, NULL, "text/html", OK);
            write(clientSocket, res.header, strlen(res.header));
        } else {
            // TODO send method not allowed
            buildResponse((struct Response *) &res, NULL, "text/text", METHOD_NOT_ALLOWED);
            write(clientSocket, res.header, strlen(res.header));
        }

        close(clientSocket);
    }
    close(serverSocket);
    free(requestBuffer);
    exit(0);
}

void buildRequest(Request *req, char* requestFromClient) {
    char* tooken, *firstLine;

    req->method = malloc(sizeof(char*) * 5);
    req->uri = malloc(sizeof(char*) * 20);

    firstLine = strtok(requestFromClient, "\n");
    tooken = strtok(firstLine, " ");
    req->method = tooken;
    req->uri = strtok(NULL, " ");

    if (DEBUG) {
        printf("METHOD: %s\n", req->method);
        printf("URI: %s\n", req->uri);
    }
}


void buildResponse(Response *res, char* body, char* contentType, char* responseCode) {
    const char* header = "HTTP/1.0 %s\n"
                         "Content-type: %s\n"
                         "Content-length: %d\n"
                         "\n";

    int bodySize = 0;
    int headerSize = strlen(header);

    if (body != NULL) {
        bodySize = strlen(body);
        res->body = (char*) malloc(bodySize);
        res->body = body;
    }

    res->header = (char*) malloc(headerSize);
    res->size = bodySize;

    // Copies the header to the response header with the parameters
    // responseCode, contentType and size of the response
    sprintf(res->header, header, responseCode, contentType, res->size);

    if (DEBUG) {
        printf("----\n");
        printf("HEADER: %s\n", res->header);
        printf("BODY: %s\n", res->body);
        printf("----\n");
    }
}
