#include <string.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <postgresql/libpq-fe.h>
#include "../../../utils/json_helper/json_helper.h"
#include "../../router/router.h"
#include "../../../utils/jwt_helper/jwt_helper.h"
#include "../../../utils/http_helper/http_helper.h"
#include "../../../utils/file_helper/file_helper.h"
#include "../../../models/models.h"
#include "../../../utils/crypt_helper/crypt_helper.h"
#include "../../../utils/datetime_helper/datetime_helper.h"
#include "../../../models/DAOs/user/user_dao.h"
void loginHandler(RouterParams params) {
    char *response = "HTTP/1.1 401 Unauthorized\r\nContent-Length: 15\r\n\r\nNot Authorized!";
    char* email = getValueFromJson(params.request.body,"email");
    char* plain_pw = getValueFromJson(params.request.body,"password");
    if(email == NULL || plain_pw == NULL){
        free(email);
        free(plain_pw);
        // 400 Bad Request
        response = "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n";
        send(params.thread_data->client_socket, response, strlen(response), 0);
        return;
    }
    char* pw = encrypt(plain_pw);
    free(plain_pw);
    User* user = authenticateUser(params.thread_data->connection,email,pw);
    free(email);
    free(pw);
    if(user!=NULL){
        TokenPayload payload;
        payload.email = user->email;
        payload.id = user->id;
        DateTime dt = datetime_addMinutes(datetime_now(), EXPIRE_MINUTES);
        payload.expire = datetime_format(dt);
        printf("Logged in, token will expire at: %s\n", payload.expire);
        free(user);
        char* token = encodeToken(&payload);
        if (token == NULL) {
            response = "HTTP/1.1 500 Internal Server Error\r\nContent-Length: 23\r\n\r\nToken generation failed!";
        } else {
            // Creazione del JSON di risposta
            JsonProperty props[] = {
                {"token", (void*)token, STRING}
            };
            char* jsonValue = formatJsonProps(props,1);

            // Creazione della risposta HTTP includendo il JSON formattato
            char responseBuffer[1024];
            snprintf(responseBuffer, sizeof(responseBuffer),
                "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: %zu\r\n\r\n%s",
                strlen(jsonValue),
                jsonValue);

            send(params.thread_data->client_socket, responseBuffer, strlen(responseBuffer), 0);

            free(jsonValue);
            free(token);
            return;
        }
    }
    send(params.thread_data->client_socket, response, strlen(response), 0);
}

void registerHandler(RouterParams params) {
    char* email = getValueFromJson(params.request.body,"email");
    char* plain_pw = getValueFromJson(params.request.body,"password");
    if(email == NULL || plain_pw == NULL){
        free(email);
        free(plain_pw);
        const char *response = "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n";
        send(params.thread_data->client_socket, response, strlen(response), 0);
        return;
    }
    char* pw = encrypt(plain_pw);
    free(plain_pw);
    User* user = registerUser(params.thread_data->connection,email,pw);
    free(email);
    free(pw);
    if(user!=NULL){
        const char *response = "HTTP/1.1 200 OK\r\nContent-Length: 0\r\n\r\n";
        send(params.thread_data->client_socket, response, strlen(response), 0);
        free(user);
        return;
    }else {
        // 500 Internal Server Error
        const char *response = "HTTP/1.1 500 Internal Server Error\r\nContent-Length: 0\r\n\r\n";
        send(params.thread_data->client_socket, response, strlen(response), 0);
    }
}

void getUserHandler(RouterParams params) {
    const char* email_par = getPathParameter(params.request.path);
    if(email_par == NULL){
        // 400 Bad Request
        const char *response = "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n";
        send(params.thread_data->client_socket, response, strlen(response), 0);
        return;
    }
    User* user = getUserByEmail(params.thread_data->connection,email_par);
    if(user != NULL){
        JsonProperty props[] = {
            {"email", (void*)user->email, STRING}, 
            //{"password", (void*)user->password, STRING},
            {"id", &user->id, INT}
            //{"float example", &val, FLOAT},
            //{"bool example", &boolValue, BOOL},
            //{"static string example","hello static",STRING}
        };

        int propsCount = sizeof(props) / sizeof(props[0]);
        char* formattedJson = formatJsonProps(props, propsCount);
        char response[1024];
        snprintf(response, sizeof(response),
            "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: %zu\r\n\r\n%s",
            strlen(formattedJson),
            formattedJson);
        send(params.thread_data->client_socket, response, strlen(response), 0);
        free(user);
        free(formattedJson);
        return;
    } else {
        const char *response = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
        send(params.thread_data->client_socket, response, strlen(response), 0);
    }
}