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

void homeHandler(RouterParams params) {
    TokenPayload* token = decodeToken(params.request.authorization);
    if(token!= NULL){
        const char* decoded_username = token->username;
        char buffer[1024];
        char body[100];
        snprintf(body, sizeof(body),"Hello, %s",decoded_username);
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
    bool user = jsonCompare(params.request.body,"user","alex");
    bool pw = jsonCompare(params.request.body,"password","123");
    if(user && pw){
        TokenPayload payload;
        payload.username = "alex";
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
    printJsonKeysAndValues(params.request.body);
    int count;
    char** list = getListFromJson(params.request.body, "list", &count);

    if (list != NULL) {
        printf("List:\n");
        for (int i = 0; i < count; i++) {
            printf("%s\n", list[i]);
        }
        for (int i = 0; i < count; i++) {
            free(list[i]);
        }
        free(list);
    } else {
        printf("Key not found in JSON.\n");
    }
    const char *response = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, World!";
    send(params.thread_data->client_socket, response, strlen(response), 0);
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