#include <string.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>

void homeHandler(int client_socket, const char *body, const char *authorization) {
    const char *response = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, World!";
    send(client_socket, response, strlen(response), 0);
}

void loginHandler(int client_socket, const char *body, const char *authorization) {
    printf("Login Body: %s\n", body);
    printf("Login Authorization: %s\n", authorization);
    
    const char *response = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, World!";
    send(client_socket, response, strlen(response), 0);
}

void registerHandler(int client_socket, const char *body, const char *authorization) {
    const char *response = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, World!";
    send(client_socket, response, strlen(response), 0);
}