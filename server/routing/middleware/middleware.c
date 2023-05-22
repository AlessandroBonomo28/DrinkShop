#include <string.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <postgresql/libpq-fe.h>
#include "middleware.h"
#include "../../utils/jwt_helper/jwt_helper.h"

void requiresAuth(HandlerParams params, void (*next)(HandlerParams params)) {
    if (verifyToken(params.authorization)) {
        next(params);
    } else {
        const char* response = "HTTP/1.1 401 Unauthorized\r\nContent-Length: 15\r\n\r\nNot Authorized!";
        send(params.client_socket, response, strlen(response), 0);
    }
}