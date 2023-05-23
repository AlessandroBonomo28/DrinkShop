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

void loginHandler(RouterParams params) {
    char *response = "HTTP/1.1 401 Unauthorized\r\nContent-Length: 15\r\n\r\nNot Authorized!";
    char* email = getValueFromJson(params.request.body,"email");
    char* pw = getValueFromJson(params.request.body,"password");
    if(email == NULL || pw == NULL){
        free(email);
        free(pw);
        // 400 Bad Request
        response = "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n";
        send(params.thread_data->client_socket, response, strlen(response), 0);
        return;
    }
    User* user = authenticateUser(params.thread_data->connection,email,pw);
    free(email);
    free(pw);
    if(user!=NULL){
        TokenPayload payload;
        payload.email = user->email;
        payload.id = user->id;
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
    char* pw = getValueFromJson(params.request.body,"password");
    if(email == NULL || pw == NULL){
        free(email);
        free(pw);
        // 400 Bad Request
        const char *response = "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n";
        send(params.thread_data->client_socket, response, strlen(response), 0);
        return;
    }
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
// TODO utility per le date: utils/date_helper
void sayHello(RouterParams params) {
    const char* stringValue = "Hello world";
    bool boolValue = false;
    float floatValue = 123.0;
    int intValue = 123;

    JsonProperty props[] = {
        {"string", (void*)stringValue, STRING}, 
        {"integer", &intValue, INT},
        {"float", &floatValue, FLOAT},
        {"boolean", &boolValue, BOOL},
        {"static string","hello static",STRING}
    };

    int propsCount = sizeof(props) / sizeof(props[0]);
    char* formattedJson = formatJsonProps(props, propsCount);

    // Creazione della risposta HTTP includendo il JSON formattato
    // %zu è il placeholder di sizeof(response)
    char response[1024];
    snprintf(response, sizeof(response),
        "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: %zu\r\n\r\n%s",
        strlen(formattedJson),
        formattedJson);

    send(params.thread_data->client_socket, response, strlen(response), 0);

    free(formattedJson);
}

void drinkImage(RouterParams params) {
    const char* path = "images/drinks/negroni.jpg";
    if(fileExists(path))
        serveFile(path,params.thread_data->client_socket);
    else {
        const char *response = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
        send(params.thread_data->client_socket, response, strlen(response), 0);
    }
}