#ifndef JSON_HELPER_H
#define JSON_HELPER_H
#include <stdbool.h>
typedef struct {
    const char* key;
    const char* value;
} JsonPair;

// Funzione per comparare chiave e valore
bool jsonCompare(const char* json, const char* desired_key, const char* desired_value);

typedef void (*JSONFoldCallback)(const char* key, const char* value, void* accumulator);

// Funzione per il parsing di un JSON e la fold dei valori
void jsonFold(const char* json, JSONFoldCallback callback, void* accumulator);

typedef void (*JSONMapCallback)(const char* key, const char* value, void* result);

// Funzione per il parsing di un JSON e la map dei valori
void jsonMap(const char* json, JSONMapCallback callback, void* result);

char* formatQueryResultToJson(PGresult* result);

const char* getValueFromJson(const char* json, const char* key);
char** getListFromJson(const char* json, const char* key, int* out_count);

bool existsKeyInJson(const char* json,const char* key);

void printJsonKeysAndValues(const char* json);

char* formatJsonPairs(JsonPair* pairs, int count);

#endif