#include <string.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <postgresql/libpq-fe.h>
#include "../../utils/json_helper/json_helper.h"
#include "../router/router.h"
#include "../../utils/jwt_helper/jwt_helper.h"
#include "../../utils/http_helper/http_helper.h"
#include "../../utils/file_helper/file_helper.h"
#include "../../models/models.h"
#include "../../utils/crypt_helper/crypt_helper.h"
#include "../../utils/datetime_helper/datetime_helper.h"

void homeHandler(RouterParams params) {
    TokenPayload* token = decodeToken(params.request.authorization);
    if(token!= NULL){
        const char* decoded_email = token->email;
        int decoded_id = token->id;
        char buffer[1024];
        char body[100];
        snprintf(body, sizeof(body),"Hello, %s. Your id:%d",decoded_email,decoded_id);
        HttpResponse response;
        response.code = "200 OK";
        response.contentType = "application/json";
        response.body = body;
        formatHttpResponse(buffer, sizeof(buffer), &response);
        send(params.thread_data->client_socket, buffer, strlen(buffer), 0);
        free(token);
    } else {
        const char *response = "HTTP/1.1 500 Server Error\r\nContent-Length: 0\r\n\r\n";
        send(params.thread_data->client_socket, response, strlen(response), 0);
    }
}
