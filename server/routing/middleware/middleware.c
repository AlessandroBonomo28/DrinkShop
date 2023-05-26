#include <string.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <postgresql/libpq-fe.h>
#include "middleware.h"
#include "../../utils/jwt_helper/jwt_helper.h"
#include "../../utils/file_helper/file_helper.h"
#include "../../utils/datetime_helper/datetime_helper.h"

void requiresAuth(RouterParams params, void (*next)(RouterParams params)) {
    if (verifyToken(params.request.authorization)) {
        TokenPayload* payload = decodeToken(params.request.authorization);
        if(payload==NULL){
            const char *response = "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n";
            send(params.thread_data->client_socket, response, strlen(response), 0);
            return;
        }
        DateTime dt = *datetime_parse(payload->expire);
        if(dt_greaterThen(datetime_now(),dt)){
            serveFileWithResponseCode( "images/401.jpg", "401 Unauthorized",params.thread_data->client_socket);
            printf("Token expired at: %s\n", payload->expire);
            free(payload);
            return;
        }
        printf("Token will expire at: %s\n", payload->expire);
        free(payload);
        next(params);
    } else {
        serveFileWithResponseCode( "images/401.jpg", "401 Unauthorized",params.thread_data->client_socket);
    }
}