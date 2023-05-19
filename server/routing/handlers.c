#include <string.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <postgresql/libpq-fe.h>
#include "../utils/json_helper.hpp"

void homeHandler(int client_socket, const char *body, const char *authorization) {
    const char *response = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, World!";
    send(client_socket, response, strlen(response), 0);
}

void foldCheckUserCallback(const char* key, const char* value, void* accumulator) {
    bool* valid = (bool*)accumulator;

    if (strcmp(key, "user") == 0 && strcmp(value, "alex") == 0) {
        *valid = true;
    } else if (strcmp(key, "password") == 0 && strcmp(value, "123") == 0) {
        *valid = true;
    }
}
// Utilizzo di jsonFold con foldCheckUserCallback
bool validateUserAndPassword(const char* json) {
    bool valid = false;
    parseJSONFold(json, foldCheckUserCallback, &valid);
    return valid;
}
void loginHandler(int client_socket, const char *body, const char *authorization) {
    printf("Login Body: %s\n", body);
    const char* json = "{\"user\":\"alex\",\"password\":\"1234\"}";
    printf("Login Authorization: %s\n", authorization);
    char *response = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nAuthorized!";
    if(!validateUserAndPassword(json)){
        response = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nNot authorized!";
        printf("NOT Authorized\n");
    }
    else printf("Authorized\n");
    
    send(client_socket, response, strlen(response), 0);
}

void registerHandler(int client_socket, const char *body, const char *authorization) {
    const char *response = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, World!";
    send(client_socket, response, strlen(response), 0);
}