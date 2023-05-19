#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <postgresql/libpq-fe.h>
#include "utils/json_helper.hpp"

#define MAX_CLIENTS 10
/*
// PRODUCTION CONFIGURATION 
#define DATABASE "host=postgres-db port=5432 dbname=drinks user=docker password=12345"
#define PORT 4040
*/


// DEBUG CONFIGURATION 
#define DATABASE "host=127.0.0.1 port=42069 dbname=drinks user=docker password=12345"
#define PORT 4040


typedef struct {
    int client_socket;
    PGconn *connection;
} ThreadData;

void *client_thread(void *arg) {
    ThreadData *thread_data = (ThreadData *)arg;

    // Esegui le operazioni del thread connesso al client
    // Utilizza thread_data->client_socket per interagire con il client
    printf("CLIENT QUERY\n");
    // Esempio di esecuzione di una query sul database
    PGresult *result = PQexec(thread_data->connection, "SELECT * FROM \"Users\";");
    printf("result json = %s\n",formatQueryResultToJson(result));
    fflush(stdout);
    if (PQresultStatus(result) == PGRES_TUPLES_OK) {
        
        int rows = PQntuples(result);
        int columns = PQnfields(result);
        printf("rows = %d\n",rows);
        fflush(stdout); 
        /* svuota il buffer (sei in un thread e le print )
        l'output di printf viene memorizzato in un buffer interno,
         che potrebbe non essere immediatamente svuotato sullo stdout. 
        */
        printf("cols = %d\n",columns);
        fflush(stdout); 
        // Invia il numero di righe al client
        char rows_str[10];
        snprintf(rows_str, sizeof(rows_str), "%d\n", rows);
        write(thread_data->client_socket, rows_str, strlen(rows_str));

        // Invia il risultato della query al client
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                const char* value = PQgetvalue(result, i, j);
                printf("%s\n",value);
                fflush(stdout); 
                write(thread_data->client_socket, value, strlen(value));
                write(thread_data->client_socket, "\t", 1);
            }
            write(thread_data->client_socket, "\n", 1);
        }
    }
    else 
    {
        printf("error");
        fflush(stdout); 
        //write(thread_data->client_socket, "ERROR", 5);
    }

    PQclear(result);

    // Chiudi la connessione al database
    PQfinish(thread_data->connection);

    // Chiudi il socket del client
    close(thread_data->client_socket);

    free(thread_data);
    pthread_exit(NULL);
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
