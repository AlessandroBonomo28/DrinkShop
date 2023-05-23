#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include "server.h"
#include "utils/json_helper/json_helper.h"
#include "utils/http_helper/http_helper.h"
#include "routing/router/router.h"

int client_count = 0;

void *client_thread(void *arg) {
    ThreadData *thread_data = (ThreadData *)arg;

    // Esegui le operazioni del thread connesso al client
    // Utilizza thread_data->client_socket per interagire con il client

    printf("(+) New client connected. (Connected now: %d)\n",++client_count);
    fflush(stdout);

    // Imposta il timeout di 5 secondi per la ricezione
    struct timeval timeout;
    timeout.tv_sec = TIMEOUT_SECONDS_ABORT_CONNECTION;
    timeout.tv_usec = 0;
    setsockopt(thread_data->client_socket, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof(timeout));

    // Buffer per la ricezione dei dati dal client
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));

    // Ricezione della richiesta dal client
    ssize_t bytesRead = recv(thread_data->client_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytesRead == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            printf("(!) Client socket timed out, closing thread. (Connected now: %d)\n",--client_count);
            fflush(stdout);
        } else {
            perror("(!) Errore nella ricezione della richiesta dal client");
            printf("(!) Client connection aborted, closing thread. (Connected now: %d)\n",--client_count);
            fflush(stdout);
        }
        PQfinish(thread_data->connection);
        close(thread_data->client_socket);
        free(thread_data);
        pthread_exit(NULL);
    }

    HttpRequest request; 
    decodeHttpRequest(buffer,&request);
    printHttpRequest(&request);

    RouterParams params;
    params.thread_data = thread_data;
    params.request = request;
    routeRequest(params);

    // Chiudi la connessione al database
    PQfinish(thread_data->connection);

    // Chiudi il socket del client
    close(thread_data->client_socket);

    free(thread_data);

    printf("(-) Client served, closing thread. (Connected now: %d)\n",--client_count);
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
        perror("(!) Errore nella creazione del socket del server");
        exit(EXIT_FAILURE);
    }

    // Configura l'indirizzo del server
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Collega il socket del server all'indirizzo
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("(!) Errore nel binding del socket del server");
        exit(EXIT_FAILURE);
    }

    // Metti il socket del server in ascolto
    if (listen(server_socket, MAX_CLIENTS) == -1) {
        perror("(!) Errore nell'ascolto del socket del server");
        exit(EXIT_FAILURE);
    }

    printf("HTTP Server listening on port %d...\n",PORT);

    while (1) {
        // Accetta una connessione dal client
        client_addr_len = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_socket == -1) {
            perror("(!) Errore nell'accettazione della connessione del client");
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
            fprintf(stderr, "(!) Il pool di connessioni è pieno, impossibile accettare nuove connessioni\n");
            close(client_socket);
            continue;
        }

        // Crea una nuova connessione al database
        connections[i] = PQconnectdb(DATABASE);
        if (PQstatus(connections[i]) != CONNECTION_OK) {
            fprintf(stderr, "(!) Errore nella connessione al database: %s\n", PQerrorMessage(connections[i]));
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
            fprintf(stderr, "(!) Errore nella creazione del thread\n");
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
    shutdown(server_socket, SHUT_RDWR);
    close(server_socket);
    return 0;
}
