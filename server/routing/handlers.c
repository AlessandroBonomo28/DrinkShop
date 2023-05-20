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

void loginHandler(int client_socket, const char *body, const char *authorization) {
    printf("Login Body: %s\n", body);
    const char* json = "{\"user\":\"alex\",\"password\":\"123\"}";
    printf("Login Authorization: %s\n", authorization);
    char *response = "HTTP/1.1 401 Unauthorized\r\nContent-Length: 15\r\n\r\nNot Authorized!";
    bool user = jsonCompare(body,"user","alex");
    bool pw = jsonCompare(body,"password","123");
    printf("pw %s\n",pw ? "true" : "false");
    printf("user %s\n",user ? "true" : "false");
    if(user && pw){
        // se la content length non matcha il count del body allora il browser
        // non riesce a interpretare la richiesta.
        // in questo caso 11 = len(Authorized!)
        response = "HTTP/1.1 200 OK\r\nContent-Length: 11\r\n\r\nAuthorized!";
        printf("Authorized\n");
    }
    else printf("NOT Authorized\n");
    
    send(client_socket, response, strlen(response), 0);
}

void registerHandler(int client_socket, const char *body, const char *authorization) {
    if(existsKeyInJSON(body,"user"))
        printf("user exists = %s\n\n",getValueFromJSON(body,"user"));
    else
        printf("user does not exist\n\n");
    

    const char *response = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, World!";
    send(client_socket, response, strlen(response), 0);
}

void requiresAuth(int client_socket, const char *body, const char *authorization, void (*next)(int client_socket, const char *body, const char *authorization)) {
    printf("Middleware auth Body: %s\n", body);
    printf("Middleware auth Authorization: %s\n", authorization);
    
    bool user = jsonCompare(body,"user","alex");
    bool pw = jsonCompare(body,"password","123");
    if(user && pw) {
        next(client_socket, body, authorization);
        printf("Authorized\n");
    }
    else {
        printf("NOT Authorized\n");
        const char *response = "HTTP/1.1 401 Unauthorized\r\nContent-Length: 15\r\n\r\nNot Authorized!";
        send(client_socket, response, strlen(response), 0);
    }
}