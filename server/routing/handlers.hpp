#ifndef HANDLERS_H
#define HANDLERS_H

void requiresAuth(int client_socket, const char *body, const char *authorization, void (*next)(int client_socket, const char *body, const char *authorization));

void homeHandler(int client_socket, const char *body, const char *authorization);
void loginHandler(int client_socket, const char *body, const char *authorization);
void registerHandler(int client_socket, const char *body, const char *authorization);

#endif