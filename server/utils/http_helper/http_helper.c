#include "http_helper.h"
#include <stdio.h>
#include <string.h>

void decodeHttpRequest(const char* buffer, HttpRequest* request) {
    sscanf(buffer, "%9s %1023s", request->method, request->path);

    char* authorizationHeader = strstr(buffer, "Authorization: ");
    if (authorizationHeader != NULL) {
        sscanf(authorizationHeader, "Authorization: Bearer %255s", request->authorization);
    }

    char* bodyStart = strstr(buffer, "\r\n\r\n");
    if (bodyStart != NULL) {
        strncpy(request->body, bodyStart + 4, sizeof(request->body) - 1);
    }
}

void printHttpRequest(const HttpRequest* request) {
    printf("Method: %s\n", request->method);
    printf("Path: %s\n", request->path);

    if (request->authorization[0] != '\0') {
        printf("Authorization (Bearer token): %s\n", request->authorization);
    } else {
        printf("Empty Authorization (Bearer token)\n");
    }

    if (request->body[0] != '\0') {
        printf("Body: %s\n", request->body);
    } else {
        printf("Empty Body\n");
    }
}

void formatHttpResponse(char* responseBuffer, size_t bufferSize, const HttpResponse* request) {
    snprintf(responseBuffer, bufferSize, "HTTP/1.1 %s\r\nContent-Type: %s\r\nContent-Length: %zu\r\n\r\n%s",
             request->code, request->contentType, strlen(request->body), request->body);
}