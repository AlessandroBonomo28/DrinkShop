#ifndef JSON_HELPER_H
#define JSON_HELPER_H
#include <stdbool.h>

// Funzione per comparare chiave e valore
bool jsonCompare(const char* json, const char* desired_key, const char* desired_value);

// Funzione di callback per la fold
typedef void (*JSONFoldCallback)(const char* key, const char* value, void* accumulator);

// Funzione per il parsing di un JSON e la fold dei valori
void jsonFold(const char* json, JSONFoldCallback callback, void* accumulator);

// Funzione di callback per la map
typedef void (*JSONMapCallback)(const char* key, const char* value, void* result);

// Funzione per il parsing di un JSON e la map dei valori
void jsonMap(const char* json, JSONMapCallback callback, void* result);

// Dichiarazioni delle funzioni JSON
char* formatQueryResultToJson(PGresult* result);
// Altre dichiarazioni delle funzioni relative a JSON

#endif