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



void getDrinkImageHandler(RouterParams params) {
    // ottieni il parametro id dalla path
    const char* str_drink_id = getPathParameter(params.request.path);
    if(str_drink_id == NULL){ 
        const char *response = "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n";
        send(params.thread_data->client_socket, response, strlen(response), 0);
        return;
    }
    Drink* drink = getDrinkById(params.thread_data->connection,atoi(str_drink_id));
    if(drink == NULL){
        // 404 Drink not found
        const char *response = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
        send(params.thread_data->client_socket, response, strlen(response), 0);
        return;
    }
    const char* path = drink->image_url;
    if(fileExists(path))
        serveFile(path,params.thread_data->client_socket);
    else {
        const char *response = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
        send(params.thread_data->client_socket, response, strlen(response), 0);
    }
    free(drink);
}

void getDrinkHandler(RouterParams params) {
    const char* str_drink_id = getPathParameter(params.request.path);
    if(str_drink_id == NULL){ 
        const char *response = "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n";
        send(params.thread_data->client_socket, response, strlen(response), 0);
        return;
    }
    Drink* drink = getDrinkById(params.thread_data->connection,atoi(str_drink_id));
    if(drink == NULL){
        // 404 Drink not found
        const char *response = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
        send(params.thread_data->client_socket, response, strlen(response), 0);
        return;
    }
    JsonProperty props[] = {
        {"name", (void*)drink->name, STRING}, 
        {"description", (void*)drink->description, STRING},
        {"price", &drink->price, FLOAT},
        {"id", &drink->id, INT},
        {"image_url", (void*)drink->image_url, STRING}
    };

    int propsCount = sizeof(props) / sizeof(props[0]);
    char* formattedJson = formatJsonProps(props, propsCount);
    char response[1024];
    snprintf(response, sizeof(response),
        "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: %zu\r\n\r\n%s",
        strlen(formattedJson),
        formattedJson);
    send(params.thread_data->client_socket, response, strlen(response), 0);
    free(drink);
    free(formattedJson);
}

void getDrinksHandler(RouterParams params) {
    PGresult* drinks_result = getDrinks(params.thread_data->connection);
    if(drinks_result == NULL){
        // 404 Drink not found
        const char *response = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
        send(params.thread_data->client_socket, response, strlen(response), 0);
        return;
    }
    char* json = formatQueryResultToJson(drinks_result);
    if( strlen(json)>= 4096) {
        // error 500 non posso mostrare tutti i drinks nel body della request
        const char *response = "HTTP/1.1 500 Internal Server Error\r\nContent-Length: 8\r\n\r\ntoo many";
        send(params.thread_data->client_socket, response, strlen(response), 0);
        free(json);
        PQclear(drinks_result);
        return;
    }
    char buffer[4096];
    HttpResponse response;
    response.code = "200 OK";
    response.contentType = "application/json";
    response.body = json;
    formatHttpResponse(buffer, sizeof(buffer), &response);
    send(params.thread_data->client_socket, buffer, strlen(buffer), 0);
    free(json);
    PQclear(drinks_result);
}