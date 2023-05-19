#ifndef JSON_HELPER_H
#define JSON_HELPER_H

// Funzione di callback per la fold
typedef void (*JSONFoldCallback)(const char* key, const char* value, void* accumulator);

// Funzione per il parsing di un JSON e la fold dei valori
void parseJSONFold(const char* json, JSONFoldCallback callback, void* accumulator);

// Funzione di callback per la map
typedef void (*JSONMapCallback)(const char* key, const char* value, void* result);

// Funzione per il parsing di un JSON e la map dei valori
void parseJSONMap(const char* json, JSONMapCallback callback, void* result);

// Dichiarazioni delle funzioni JSON
char* formatQueryResultToJson(PGresult* result);
// Altre dichiarazioni delle funzioni relative a JSON

#endif