#ifndef SERVER_H
#define SERVER_H
#include <postgresql/libpq-fe.h>
// PRODUCTION CONFIGURATION 
//#define DATABASE "host=postgres-db port=5432 dbname=drinks user=docker password=12345"

// DEBUG CONFIGURATION 
#define DATABASE "host=127.0.0.1 port=42069 dbname=drinks user=docker password=12345"
#define PORT 4040
#define MAX_CLIENTS 100
#define TIMEOUT_SECONDS_ABORT_CONNECTION 5
typedef struct {
    int client_socket;
    PGconn *connection;
} ThreadData;

#endif