#include <string.h>
#include <sys/socket.h>
#include "router.h"
#include "../handlers/handlers.h"
#include "../middleware/middleware.h"
#define NO_MIDDLEWARE NULL
#define MAX_PATH_LENGTH 1024
#define ROUTES_PREFIX "" // for example "/api"

Route routes[] = {
    { "GET", "/", requiresAuth, homeHandler },
    { "POST", "/login", NO_MIDDLEWARE, loginHandler },
    { "POST", "/register", NO_MIDDLEWARE, registerHandler },
    { "GET", "/hello", requiresAuth, sayHello },
    { "GET", "/drink/image", requiresAuth, drinkImage } // TODO /drink/image/:id (convenzione /<var>/<value>)
    // TODO POST /order
    // TODO GET /drinks
    // TODO GET /user/:id (esempio user/1)
    // TODO GET /drinks/:datetime/:user_id (drinks ordinati il giorno date, dall'utente id drinks/30-02-2001:00:00:00/1)
    // altre routes...
};

void routeRequest(RouterParams params) {
    int numRoutes = sizeof(routes) / sizeof(Route);
    for (int i = 0; i < numRoutes; i++) {
        char routePath[MAX_PATH_LENGTH];
        snprintf(routePath, sizeof(routePath), "%s%s", ROUTES_PREFIX, routes[i].path);
        if (strcmp(params.request.method, routes[i].method) == 0 && strcmp(params.request.path, routePath) == 0) {
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
    send(params.thread_data->client_socket, response, strlen(response), 0);
}
