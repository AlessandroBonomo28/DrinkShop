#include <string.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <postgresql/libpq-fe.h>
#include "../../utils/json_helper/json_helper.h"
#include "../router/router.h"
#include "../../utils/jwt_helper/jwt_helper.h"

void homeHandler(RequestParams params) {
    const char *response = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nWelcome, user";
    send(params.client_socket, response, strlen(response), 0);
}

void loginHandler(RequestParams params) {
    char *response = "HTTP/1.1 401 Unauthorized\r\nContent-Length: 15\r\n\r\nNot Authorized!";
    bool user = jsonCompare(params.body,"user","alex");
    bool pw = jsonCompare(params.body,"password","123");
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

            send(params.client_socket, responseBuffer, strlen(responseBuffer), 0);

            free(jsonValue);
            free(token);
            return;
        }
    }
    send(params.client_socket, response, strlen(response), 0);
}

void registerHandler(RequestParams params) {
    printJsonKeysAndValues(params.body);
    int count;
    char** list = getListFromJson(params.body, "list", &count);

    if (list != NULL) {
        printf("List:\n");
        for (int i = 0; i < count; i++) {
            printf("%s\n", list[i]);
        }

        // Deallocazione della memoria
        for (int i = 0; i < count; i++) {
            free(list[i]);
        }
        free(list);
    } else {
        printf("Key not found in JSON.\n");
    }
    const char *response = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, World!";
    send(params.client_socket, response, strlen(response), 0);
}
// TODO utility per le date: utils/date_helper
// TODO utility per jwt: utils/jwt_helper
void sayHello(RequestParams params) {
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

    send(params.client_socket, response, strlen(response), 0);

    free(formattedJson);
}