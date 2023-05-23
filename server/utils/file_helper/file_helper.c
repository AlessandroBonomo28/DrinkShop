#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdbool.h>
#include "file_helper.h"

#define BUFFER_SIZE 1024

bool fileExists(const char* path) {
    FILE* file = fopen(path, "rb");
    if (file != NULL) {
        fclose(file);
        return true;
    }
    return false;
}
void serveFileWithResponseCode(const char* path,const char* response_code, int client_socket)
{
    // Apri il file in modalità di lettura binaria
    FILE* file = fopen(path, "rb");
    if (file == NULL) {
        printf("Errore: Impossibile aprire il file %s\n", path);
        return;
    }

    // Determina la dimensione del file
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Determina il tipo di contenuto in base all'estensione del file
    const char* contentType;
    const char* fileExtension = strrchr(path, '.');
    if (fileExtension != NULL) {
        if (strcasecmp(fileExtension, ".jpg") == 0 || strcasecmp(fileExtension, ".jpeg") == 0) {
            contentType = "image/jpeg";
        } else if (strcasecmp(fileExtension, ".png") == 0) {
            contentType = "image/png";
        } else if (strcasecmp(fileExtension, ".gif") == 0) {
            contentType = "image/gif";
        } else {
            contentType = "application/octet-stream";
        }
    } else {
        contentType = "application/octet-stream";
    }

    // Invia l'intestazione HTTP al browser con il tipo di contenuto corretto
    char header[256];
    snprintf(header, sizeof(header), "HTTP/1.1 %s\r\nContent-Type: %s\r\nContent-Length: %ld\r\n\r\n",response_code, contentType, fileSize);
    //snprintf(header, sizeof(header), "HTTP/1.1 200 OK\r\nContent-Type: %s\r\nContent-Length: %ld\r\n\r\n", contentType, fileSize);
    send(client_socket, header, strlen(header), 0);

    // Leggi e invia il contenuto del file sulla socket
    char buffer[BUFFER_SIZE];
    size_t bytesRead;
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        send(client_socket, buffer, bytesRead, 0);
    }

    // Chiudi il file dopo averlo letto
    fclose(file);

}
void serveFile(const char* path, int client_socket) {
    serveFileWithResponseCode(path, "200 OK", client_socket);
}

