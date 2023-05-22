#include <string.h>
#include <stdlib.h>
#include "jwt_helper.h"
#include <jwt.h>

char* encodeToken(const TokenPayload* payload) {
    jwt_t* jwt = NULL;
    jwt_new(&jwt);
    jwt_add_grant(jwt, "email", payload->email);
    jwt_add_grant_int(jwt, "id", (long)payload->id);
    /* ipotetici attributi di tipo bool e float
    //jwt_add_grant_bool(jwt, "premium", (int)payload->premium);
    //jwt_add_grant_double(jwt, "id", (double)payload->id);
    */
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
        
        const char* email = jwt_get_grant(decoded, "email");
        if (email != NULL) {
            payload->email = strdup(email);
        } else {
            // Errore nel recuperare l'attributo "email" dal token
            free(payload);
            payload = NULL;
            jwt_free(decoded);
            return NULL;
        }

        long id = jwt_get_grant_int(decoded, "id");
        payload->id = (int)id;

        /*
        // ipotetico attributo bool
        int premium = jwt_get_grant_bool(decoded, "premium");
        payload->premium = (bool)premium;

        // ipotetico attributo float
        double val = jwt_get_grant_double(decoded, "val");
        payload->val = (float)val;
        */
        jwt_free(decoded);
        return payload;
    } else {
        return NULL;
    }
}

