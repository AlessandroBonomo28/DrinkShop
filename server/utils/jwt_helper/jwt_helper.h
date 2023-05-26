#ifndef JWT_HELPER_H
#define JWT_HELPER_H

#include <stdbool.h>

#define SECRET "YOUR_SECRET_HERE"
#define EXPIRE_MINUTES 45
typedef struct {
    int id;
    const char* email;
    const char* expire;
} TokenPayload;

char* encodeToken(const TokenPayload* payload);
bool verifyToken(const char* token);
TokenPayload* decodeToken(const char* token);

#endif 
