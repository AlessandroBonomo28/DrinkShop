#include <string.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <postgresql/libpq-fe.h>
#include "../utils/json_helper.hpp"

void homeHandler(int client_socket, const char *body, const char *authorization) {
    const char *response = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, World!";
    send(client_socket, response, strlen(response), 0);
}
void foldPrintKeyValueCount(const char* key, const char* value, void* accumulator) {
    // Cast dell'accumulatore al tipo corretto (nel nostro caso, un puntatore a intero)
    int* count = (int*)accumulator;

    // Stampa la chiave e il valore
    printf("Chiave: %s\n", key);
    printf("Valore: %s\n", value);

    // Incrementa il conteggio
    (*count)++;
}

void loginHandler(int client_socket, const char *body, const char *authorization) {
    printf("Login Body: %s\n", body);
    const char* json = "{\"user\":\"alex\",\"password\":\"123\"}";
    printf("Login Authorization: %s\n", authorization);
    int*a = 0;
    jsonFold(json,foldPrintKeyValueCount,&a);
    printf("val = %d",a);
    char *response = "HTTP/1.1 401 Unauthorized\r\nContent-Length: 13\r\n\r\nNot Authorized!";
    const char* desired_value = "alex";
    const char* desired_key = "uer";
    bool user = jsonCompare(json,desired_key,desired_value);
    bool pw = jsonCompare(json,"password","1235");
    printf("pw %s\n",pw ? "true" : "false");
    printf("user %s\n",user ? "true" : "false");
    if(user && pw){
        response = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nAuthorized!";
        printf("Authorized\n");
    }
    else printf("NOT Authorized\n");
    
    send(client_socket, response, strlen(response), 0);
}

void registerHandler(int client_socket, const char *body, const char *authorization) {
    const char *response = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, World!";
    send(client_socket, response, strlen(response), 0);
}