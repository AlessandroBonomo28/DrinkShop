#include <string.h>
#include <stdlib.h>
#include "jwt_helper.h"
#include <jwt.h>

char* encodeToken(const TokenPayload* payload) {
    jwt_t* jwt = NULL;
    jwt_new(&jwt);
    jwt_add_grant(jwt, "username", payload->username); // TODO generalizzare encode payload jwt_add_grant_bool
    jwt_set_alg(jwt, JWT_ALG_HS256, SECRET, strlen(SECRET));
    char* token = jwt_encode_str(jwt);
    jwt_free(jwt);
    return token;
}

bool verifyToken(const char* token) {
    jwt_t* decoded = NULL;
    jwt_decode(&decoded, token, SECRET, strlen(SECRET));
    
    if (decoded != NULL) {
        jwt_free(decoded);
        return true;
    } else {
        return false;
    } 
}

TokenPayload* decodeToken(const char* token) {
    jwt_t* decoded = NULL;
    jwt_decode(&decoded, token, SECRET, strlen(SECRET));
    
    if (decoded != NULL) {
        TokenPayload* payload = (TokenPayload*)malloc(sizeof(TokenPayload));
        // TODO generalizzare decode payload
        const char* username = jwt_get_grant(decoded, "username");
        if (username != NULL) {
            payload->username = strdup(username);
        } else {
            // Errore nel recuperare l'attributo "username" dal token
            free(payload);
            payload = NULL;
            jwt_free(decoded);
            return NULL;
        }
        
        jwt_free(decoded);
        return payload;
    } else {
        return NULL;
    }
}

