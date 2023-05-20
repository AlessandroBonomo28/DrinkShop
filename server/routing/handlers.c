#include <string.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <postgresql/libpq-fe.h>
#include "../utils/json_helper.hpp"
#include "router.hpp"

void homeHandler(RequestParams params) {
    const char *response = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, World!";
    send(params.client_socket, response, strlen(response), 0);
}

void loginHandler(RequestParams params) {
    char *response = "HTTP/1.1 401 Unauthorized\r\nContent-Length: 15\r\n\r\nNot Authorized!";
    bool user = jsonCompare(params.body,"user","alex");
    bool pw = jsonCompare(params.body,"password","123");
    if(user && pw){
        // se la content length non matcha il count del body allora il browser
        // non riesce a interpretare la richiesta.
        // in questo caso 11 = len(Authorized!)
        response = "HTTP/1.1 200 OK\r\nContent-Length: 11\r\n\r\nAuthorized!";
        printf("Authorized\n");
    }
    else printf("NOT Authorized\n");
    
    send(params.client_socket, response, strlen(response), 0);
}

void registerHandler(RequestParams params) {
    if(existsKeyInJSON(params.body,"user"))
        printf("user exists = %s\n\n",getValueFromJSON(params.body,"user"));
    else
        printf("user does not exist\n\n");
    

    const char *response = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, World!";
    send(params.client_socket, response, strlen(response), 0);
}
// TODO creare una cartella per ogni c,hpp
// TODO mettere middleware in file separato
void requiresAuth(RequestParams params, void (*next)(RequestParams params)) {
    printf("Middleware auth Body: %s\n", params.body);
    printf("Middleware auth Authorization: %s\n", params.authorization);
    

    if(strcmp(params.authorization,"secret")==0) {
        next(params);
        printf("Authorized\n");
    }
    else {
        printf("NOT Authorized\n");
        const char *response = "HTTP/1.1 401 Unauthorized\r\nContent-Length: 15\r\n\r\nNot Authorized!";
        send(params.client_socket, response, strlen(response), 0);
    }
}