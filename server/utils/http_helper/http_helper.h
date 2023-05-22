#ifndef HTTP_HELPER_H
#define HTTP_HELPER_H
#include <stddef.h>

#define HTTP_CODE_OK "200 OK"
#define HTTP_CODE_CREATED "201 Created"
#define HTTP_CODE_NO_CONTENT "204 No Content"
#define HTTP_CODE_BAD_REQUEST "400 Bad Request"
#define HTTP_CODE_UNAUTHORIZED "401 Unauthorized"
#define HTTP_CODE_FORBIDDEN "403 Forbidden"
#define HTTP_CODE_NOT_FOUND "404 Not Found"
#define HTTP_CODE_METHOD_NOT_ALLOWED "405 Method Not Allowed"
#define HTTP_CODE_INTERNAL_SERVER_ERROR "500 Internal Server Error"

#define HTTP_CONTENT_TYPE_JSON "application/json"
#define HTTP_CONTENT_TYPE_HTML "text/html"
#define HTTP_CONTENT_TYPE_PLAIN_TEXT "text/plain"
#define HTTP_CONTENT_TYPE_XML "application/xml"

#define HTTP_RESPONSE_BUFFER_SIZE 1024
#define METHOD_SIZE 10
#define PATH_SIZE 1024
#define BODY_SIZE 1024
#define AUTHORIZATION_SIZE 256

typedef struct {
    char method[METHOD_SIZE];
    char path[PATH_SIZE];
    char body[BODY_SIZE];
    char authorization[AUTHORIZATION_SIZE];
} HttpRequest;


typedef struct {
    const char* code;
    const char* contentType;
    const char* body;
} HttpResponse;

void decodeHttpRequest(const char* buffer, HttpRequest* request);
void printHttpRequest(const HttpRequest* request);
void formatHttpResponse(char* responseBuffer, size_t bufferSize, const HttpResponse* request);

#endif