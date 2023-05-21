#include <string.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <postgresql/libpq-fe.h>
#include "middleware.h"

void requiresAuth(RequestParams params, void (*next)(RequestParams params)) {
    printf("Middleware auth Body: %s\n", params.body);
    printf("Middleware auth Authorization: %s\n", params.authorization);
    
    // TODO autorizza con jwt
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