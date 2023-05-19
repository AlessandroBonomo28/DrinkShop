#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"  // Indirizzo IP del server
#define SERVER_PORT 4040      // Porta del server
#define BUFFER_SIZE 1024

int main() {
    // Creazione del socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        perror("Errore nella creazione del socket");
        exit(1);
    }

    // Creazione dell'indirizzo del server
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &(serverAddress.sin_addr)) <= 0) {
        perror("Errore nella conversione dell'indirizzo IP");
        exit(1);
    }

    // Connessione al server
    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        perror("Errore nella connessione al server");
        exit(1);
    }

    // Invio e ricezione dei dati con il server
    // ... inserisci qui il codice per l'invio e la ricezione dei dati ...

    // Ricezione del risultato dal server
    char buffer[BUFFER_SIZE];
    ssize_t totalBytesRead = 0;
    ssize_t bytesRead;

    while (totalBytesRead < BUFFER_SIZE - 1) {
        bytesRead = recv(clientSocket, buffer + totalBytesRead, BUFFER_SIZE - 1 - totalBytesRead, 0);
        if (bytesRead == -1) {
            perror("Errore nella ricezione del risultato dal server");
            exit(1);
        } else if (bytesRead == 0) {
            break;  // Connessione chiusa dal server
        }

        totalBytesRead += bytesRead;
    }

    buffer[totalBytesRead] = '\0';

    // Stampa del risultato ricevuto
    printf("Risultato della query:\n%s\n", buffer);

    // Chiusura del socket
    close(clientSocket);

    return 0;
}
