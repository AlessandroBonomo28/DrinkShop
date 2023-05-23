#include <string.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <postgresql/libpq-fe.h>
#include "middleware.h"
#include "../../utils/jwt_helper/jwt_helper.h"
#include "../../utils/file_helper/file_helper.h"

void requiresAuth(RouterParams params, void (*next)(RouterParams params)) {
    if (verifyToken(params.request.authorization)) {
        next(params);
    } else {
        serveFileWithResponseCode( "images/401.jpg", "401 Unauthorized",params.thread_data->client_socket);
    }
}