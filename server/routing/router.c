#include <string.h>
#include <sys/socket.h>
#include "router.hpp"
#include "handlers.hpp"

#define NO_MIDDLEWARE NULL

Route routes[] = {
    { "GET", "/", requiresAuth, homeHandler },
    { "POST", "/login", NO_MIDDLEWARE, loginHandler },
    { "GET", "/register", NO_MIDDLEWARE, registerHandler },
    // altre routes...
};

void routeRequest(int client_socket, const char *method, const char *path, const char *body, const char *authorization) {
    int numRoutes = sizeof(routes) / sizeof(Route);
    RequestParams params;
    params.client_socket = client_socket;
    params.body = body;
    params.authorization = authorization;
    for (int i = 0; i < numRoutes; i++) {
        if (strcmp(method, routes[i].method) == 0 && strcmp(path, routes[i].path) == 0) {
            if (routes[i].middleware == NO_MIDDLEWARE) {
                // Se non è presente il middleware, chiama direttamente la funzione handler
                routes[i].handler(params);
            } else { 
                // Altrimenti esegue il middleware
                routes[i].middleware(params, routes[i].handler);
            }
            return;
        }
    }

    // Se nessuna route corrispondente è stata trovata, invia una risposta 404 Not Found al client
    const char *response = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
    send(client_socket, response, strlen(response), 0);
}
