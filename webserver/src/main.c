#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

// Our includes
#include "status_codes.h"
#include "logging.h"

// Definitions
#define BACKLOG 10
#define BUFF_SIZE 1024

/**
 * Binds the socket.
 * Than listen on the port
 * @return [description]
 *
 */
int bindListen();

/**
 * Concates two char pointers to one charpointer
 * @param  str1
 * @param  str2
 */
char* concateStr(const char* str1, const char* str2);

int main(int argc, char** argv) {
    int port = 1337;
    // TODO config stuff

    // create socket
    int serverSocket, clientSocket;
    socklen_t addrlen;
    char *requestBuffer = malloc(BUFF_SIZE);
    struct sockaddr_in address;
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

        char* responseToClient = "Hello world\n";

        // todo read from file.
        write(clientSocket, responseToClient, strlen(responseToClient));
        close(clientSocket);
    }
    close(serverSocket);
    exit(0);
}

char* concateStr(const char* str1, const char* str2) {
    char* strToReturn;

    strToReturn = (char*)malloc(strlen(str1) + strlen(str2));
    strcpy(strToReturn, str1);
    strcat(strToReturn, str2);

    return strToReturn;
}
