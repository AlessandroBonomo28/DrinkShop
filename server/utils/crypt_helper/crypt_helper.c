#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <crypt.h>

#define SALT_SIZE 16
#define HASH_SIZE 64
char* encrypt(const char* data) {
    char salt[SALT_SIZE + 1];
    srand(471742);
    char* hash;
    for (int i = 0; i < SALT_SIZE; i++) {
        salt[i] = 'a' + rand() % 26; 
    }
    salt[SALT_SIZE] = '\0';
    hash = crypt(data, salt);
    return strdup(hash); 
}