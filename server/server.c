#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <postgresql/libpq-fe.h>
#include "utils/json_helper/json_helper.h"
#include "routing/router/router.h"
// PRODUCTION CONFIGURATION 
//#define DATABASE "host=postgres-db port=5432 dbname=drinks user=docker password=12345"

// DEBUG CONFIGURATION 
#define DATABASE "host=127.0.0.1 port=42069 dbname=drinks user=docker password=12345"
#define PORT 4040
#define MAX_CLIENTS 100

typedef struct {
    int client_socket;
    PGconn *connection;
} ThreadData;

void *client_thread(void *arg) {
    ThreadData *thread_data = (ThreadData *)arg;

    // Esegui le operazioni del thread connesso al client
    // Utilizza thread_data->client_socket per interagire con il client

    printf("NEW CLIENT CONNECTED\n");
    fflush(stdout);

    // Imposta il timeout di 5 secondi per la ricezione
    struct timeval timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;
    setsockopt(thread_data->client_socket, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof(timeout));

    // Buffer per la ricezione dei dati dal client
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));

    // Ricezione della richiesta dal client
    ssize_t bytesRead = recv(thread_data->client_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytesRead == -1) {
        perror("Errore nella ricezione della richiesta dal client");
        close(thread_data->client_socket);
        free(thread_data);
        pthread_exit(NULL);
    }

    // Decodifica della richiesta HTTP
    char method[10];
    char path[1024];
    char body[1024];
    char authorization[256];

    sscanf(buffer, "%9s %1023s", method, path);

    // Esempio di stampa dei dati ricevuti
    printf("Metodo: %s\n", method);
    printf("Percorso: %s\n", path);

    // Ottieni l'intestazione di autorizzazione (Authorization)
    char *authorizationHeader = strstr(buffer, "Authorization: ");
    if (authorizationHeader != NULL) {
        sscanf(authorizationHeader, "Authorization: Bearer %255s", authorization);
        printf("Authorization: %s\n", authorization);
    }

    // Ottieni il corpo della richiesta (se presente)
    char *bodyStart = strstr(buffer, "\r\n\r\n");
    if (bodyStart != NULL) {
        strncpy(body, bodyStart + 4, sizeof(body) - 1);
        printf("Body: %s\n", body);
    }

    routeRequest(thread_data->client_socket,method,path,body,authorization);
    //TODO togliere la query da qua e fare route di select users
    // Esempio di esecuzione di una query sul database
    PGresult *result = PQexec(thread_data->connection, "SELECT * FROM \"Users\";");

    if (PQresultStatus(result) == PGRES_TUPLES_OK) {
        int rows = PQntuples(result);
        int columns = PQnfields(result);
        const char * json_result = formatQueryResultToJson(result);

        printf("result json = %s\n",json_result);
        fflush(stdout);

        // Invia il risultato al client
        //write(thread_data->client_socket,json_result,strlen(json_result));

        // Creazione della risposta HTTP con il risultato JSON nel corpo
        char response[4096];
        snprintf(response, sizeof(response), "HTTP/1.1 200 OK\r\nContent-Length: %zu\r\n\r\n%s", strlen(json_result), json_result);
        
        // Invia la risposta al client
        //send(thread_data->client_socket, response, strlen(response), 0);
    }
    else 
    {
        printf("Errore durante l'esecuzione della query");
        fflush(stdout); 
        // Gestione dell'errore
        const char *error_response = "HTTP/1.1 500 Internal Server Error\r\nContent-Length: 0\r\n\r\n";
        send(thread_data->client_socket, error_response, strlen(error_response), 0);
    }

    PQclear(result);

    // Chiudi la connessione al database
    PQfinish(thread_data->connection);

    // Chiudi il socket del client
    close(thread_data->client_socket);

    free(thread_data);

    printf("CLIENT DISCONNECTED\n");
    fflush(stdout);
    
    pthread_exit(NULL); // Chiudi thread 
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len;

    // Inizializza il pool di connessioni
    PGconn *connections[MAX_CLIENTS];
    memset(connections, 0, sizeof(connections));

    // Inizializza il thread pool
    pthread_t thread_pool[MAX_CLIENTS];

    // Crea il socket del server
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Errore nella creazione del socket del server");
        exit(EXIT_FAILURE);
    }

    // Configura l'indirizzo del server
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Collega il socket del server all'indirizzo
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Errore nel binding del socket del server");
        exit(EXIT_FAILURE);
    }

    // Metti il socket del server in ascolto
    if (listen(server_socket, MAX_CLIENTS) == -1) {
        perror("Errore nell'ascolto del socket del server");
        exit(EXIT_FAILURE);
    }

    printf("Server in ascolto...\n");

    while (1) {
        // Accetta una connessione dal client
        client_addr_len = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_socket == -1) {
            perror("Errore nell'accettazione della connessione del client");
            continue;
        }

        // Cerca un'istanza di connessione libera nel pool
        int i;
        for (i = 0; i < MAX_CLIENTS; i++) {
            if (connections[i] == NULL || PQstatus(connections[i]) == CONNECTION_BAD) {
                break;
            }
        }

        if (i == MAX_CLIENTS) {
            fprintf(stderr, "Il pool di connessioni è pieno, impossibile accettare nuove connessioni\n");
            close(client_socket);
            continue;
        }

        // Crea una nuova connessione al database
        connections[i] = PQconnectdb(DATABASE);
        if (PQstatus(connections[i]) != CONNECTION_OK) {
            fprintf(stderr, "Errore nella connessione al database: %s\n", PQerrorMessage(connections[i]));
            PQfinish(connections[i]);
            close(client_socket);
            continue;
        }

        // Crea una struttura dati per il thread
        ThreadData *thread_data = (ThreadData *)malloc(sizeof(ThreadData));
        thread_data->client_socket = client_socket;
        thread_data->connection = connections[i];

        // Crea un nuovo thread per gestire la connessione del client
        if (pthread_create(&thread_pool[i], NULL, client_thread, (void *)thread_data) != 0) {
            fprintf(stderr, "Errore nella creazione del thread\n");
            PQfinish(connections[i]);
            close(client_socket);
            free(thread_data);
        }
    }

    // Chiudi le connessioni nel pool
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (connections[i] != NULL) {
            PQfinish(connections[i]);
        }
    }

    // Chiudi il socket del server
    close(server_socket);

    return 0;
}
