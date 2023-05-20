#ifndef ROUTER_H
#define ROUTER_H

typedef struct {
    int client_socket;
    const char *body;
    const char *authorization;
} RequestParams;

typedef struct {
    const char *method;
    const char *path;
    void (*middleware)(RequestParams params, void (*next)(RequestParams params));
    void (*handler)(RequestParams params);
} Route;


void routeRequest(int client_socket, const char *method, const char *path, const char *body, const char *authorization);


#endif