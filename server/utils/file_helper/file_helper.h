#ifndef FILE_HELPER_H
#define FILE_HELPER_H

bool fileExists(const char* path);
void serveFile(const char* path, int client_socket);
#endif