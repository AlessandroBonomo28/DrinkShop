#ifndef FILE_HELPER_H
#define FILE_HELPER_H

bool fileExists(const char* path);
void serveFileWithResponseCode(const char* path,const char* response_code, int client_socket);
void serveFile(const char* path, int client_socket);
#endif