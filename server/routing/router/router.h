#ifndef ROUTER_H
#define ROUTER_H
#include "../../server.h"
#include "../../utils/http_helper/http_helper.h"
typedef struct {
    ThreadData* thread_data;
    HttpRequest request;
} RouterParams;

typedef struct {
    const char *method;
    const char *path;
    void (*middleware)(RouterParams params, void (*next)(RouterParams params));
    void (*handler)(RouterParams params);
} Route;


void routeRequest(RouterParams params);


#endif