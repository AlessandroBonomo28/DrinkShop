#ifndef ROUTER_H
#define ROUTER_H
#include "../../server.h"
typedef struct {
    int client_socket;
    PGconn *connection;
    const char *body;
    const char *authorization;
} HandlerParams;

typedef struct {
    const char *method;
    const char *path;
    void (*middleware)(HandlerParams params, void (*next)(HandlerParams params));
    void (*handler)(HandlerParams params);
} Route;


void routeRequest(int client_socket,PGconn *connection, const char *method, const char *path, const char *body, const char *authorization);


#endif