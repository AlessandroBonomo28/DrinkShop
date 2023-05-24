#include <stdbool.h>
#include <string.h>
#include <sys/socket.h>
#include "router.h"
#include "../handlers/handlers.h"
#include "../middleware/middleware.h"
#include "../../utils/file_helper/file_helper.h"
#define NO_MIDDLEWARE NULL
#define MAX_PATH_LENGTH 1024
#define ROUTES_PREFIX "" // for example "/api"

Route routes[] = {
    { "POST", "/login", NO_MIDDLEWARE, loginHandler },
    { "POST", "/register", NO_MIDDLEWARE, registerHandler },
    { "GET", "/", requiresAuth, homeHandler },
    { "GET", "/user/:id", requiresAuth, getUserHandler }, 
    { "GET", "/drink/image/:id", NO_MIDDLEWARE, getDrinkImageHandler },
    { "GET", "/drink/:id", NO_MIDDLEWARE, getDrinkHandler },
    { "GET", "/drinks", NO_MIDDLEWARE, getDrinksHandler },
    { "POST", "/order/drink", requiresAuth, orderDrinkHandler },
    //{ "PUT", "/order/drink/:id", requiresAuth, updateDrinkQuantityInOrderHandler }, // TODO
    //{ "DELETE", "/order/:id", requiresAuth, deleteOrderHandler }, // TODO
    //{ "DELETE", "/order/drink/:id", requiresAuth, deleteDrinkFromOrderHandler }, //TODO
    { "GET", "/order/last", requiresAuth, getLastOrderMadeByUserHandler },
    { "GET", "/orders", requiresAuth, getOrdersMadeByUserHandler },
    { "GET", "/order/:id", requiresAuth, getOrderHandler },
    // TODO POST /pay (paga l'ordine corrente) auth
};

void routeRequest(RouterParams params) {
    int numRoutes = sizeof(routes) / sizeof(Route);
    for (int i = 0; i < numRoutes; i++) {
        char routePath[MAX_PATH_LENGTH];
        snprintf(routePath, sizeof(routePath), "%s%s", ROUTES_PREFIX, routes[i].path);
        if (strcmp(params.request.method, routes[i].method) == 0 && matchesPath(params.request.path, routePath)) {
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
    serveFileWithResponseCode( "images/404.jpg", "404 Not Found",params.thread_data->client_socket);
}

// match per routes con sintassi "basepath/:param"
// esempio: /user/:id deve matchare con la request /user/1 ma
// non deve matchare con /user/1/orders
bool matchesPath(const char* requestPath, const char* routePath) {
    int requestPathLen = strlen(requestPath);
    int routePathLen = strlen(routePath);

    // Controlla se il routePath ha un parametro dinamico
    const char* paramPos = strstr(routePath, "/:");
    if (paramPos == NULL) {
        return strcmp(requestPath, routePath) == 0;  // Nessun parametro dinamico nel routePath, confronta i percorsi direttamente
    }

    // Controlla se il routePath corrisponde al percorso della richiesta con un parametro dinamico
    int paramIdx = paramPos - routePath + 1;  // Indice del primo carattere del parametro (ignorando "/")
    int staticPartLen = paramIdx - 1;  // Lunghezza della parte statica del percorso (ignorando "/")
    int dynamicPartLen = routePathLen - paramIdx - 1;  // Lunghezza della parte dinamica del percorso (ignorando "/:")
    bool match = false;

    if (requestPathLen >= staticPartLen + dynamicPartLen) {
        // Verifica se la parte statica corrisponde
        if (strncmp(requestPath, routePath, staticPartLen) == 0) {
            match = true;
            // Verifica se la parte dinamica corrisponde
            const char* requestDynamicPart = requestPath + staticPartLen;
            const char* routeDynamicPart = routePath + paramIdx;
            int remainingLen = requestPathLen - staticPartLen - dynamicPartLen;
            if (remainingLen > 0 && strncmp(requestDynamicPart, routeDynamicPart, remainingLen) != 0) {
                match = false;
            }
        }
    }

    return match;
}



const char* getPathParameter(const char* path) {
    if (path == NULL || *path == '\0') {
        return NULL;
    }
    const char* lastSlash = strrchr(path, '/');
    if (lastSlash != NULL && *(lastSlash + 1) != '\0') {
        return lastSlash + 1;
    } else {
        return NULL;
    }
}
