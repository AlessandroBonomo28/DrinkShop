#ifndef ROUTER_H
#define ROUTER_H

void routeRequest(int client_socket, const char *method, const char *path, const char *body, const char *authorization);


#endif