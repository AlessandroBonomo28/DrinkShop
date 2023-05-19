#include <string.h>
#include <sys/socket.h>
#include "handlers.hpp"

typedef struct {
    const char *method;
    const char *path;
    void (*handler)(int client_socket, const char *body, const char *authorization);
} Route;

Route routes[] = {
    { "GET", "/", homeHandler },
    { "POST", "/login", loginHandler },
    { "GET", "/register", registerHandler },
    // altre routes...
};

void routeRequest(int client_socket, const char *method, const char *path, const char *body, const char *authorization) {
    int numRoutes = sizeof(routes) / sizeof(Route);

    for (int i = 0; i < numRoutes; i++) {
        if (strcmp(method, routes[i].method) == 0 && strcmp(path, routes[i].path) == 0) {
            // Richiama la funzione di callback per gestire la richiesta
            routes[i].handler(client_socket, body, authorization);
            return;
        }
    }

    // Se nessuna route corrispondente è stata trovata, invia una risposta 404 Not Found al client
    const char *response = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
    send(client_socket, response, strlen(response), 0);
}




