#ifndef JWT_HELPER_H
#define JWT_HELPER_H

#include <stdbool.h>

#define SECRET "YOUR_SECRET_HERE"

typedef struct {
    int id;
    const char* email;
} TokenPayload;

char* encodeToken(const TokenPayload* payload);
bool verifyToken(const char* token);
TokenPayload* decodeToken(const char* token);

#endif 
