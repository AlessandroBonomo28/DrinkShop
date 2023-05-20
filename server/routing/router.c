#include <string.h>
#include <sys/socket.h>
#include "handlers.hpp"

#define NO_MIDDLEWARE NULL
//TODO sistema i tipi con typedef per ridurre il numero di parametri di middleware e delle routes, il router non fa niente
typedef struct {
    const char *method;
    const char *path;
    void (*middleware)(int client_socket, const char *body, const char *authorization, void (*next)(int client_socket, const char *body, const char *authorization));
    void (*handler)(int client_socket, const char *body, const char *authorization);
} Route;

Route routes[] = {
    { "GET", "/", requiresAuth, homeHandler },
    { "POST", "/login", NO_MIDDLEWARE, loginHandler },
    { "GET", "/register", NO_MIDDLEWARE, registerHandler },
    // altre routes...
};

void routeRequest(int client_socket, const char *method, const char *path, const char *body, const char *authorization) {
    int numRoutes = sizeof(routes) / sizeof(Route);

    for (int i = 0; i < numRoutes; i++) {
        if (strcmp(method, routes[i].method) == 0 && strcmp(path, routes[i].path) == 0) {
            if (routes[i].middleware == NO_MIDDLEWARE) {
                // Se non è presente il middleware, chiama direttamente la funzione handler
                routes[i].handler(client_socket, body, authorization);
            } else { 
                // Altrimenti esegue il middleware
                routes[i].middleware(client_socket, body, authorization, routes[i].handler);
            }
            return;
        }
    }

    // Se nessuna route corrispondente è stata trovata, invia una risposta 404 Not Found al client
    const char *response = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
    send(client_socket, response, strlen(response), 0);
}
