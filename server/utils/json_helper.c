#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <postgresql/libpq-fe.h>
#include <json-c/json.h>
#include <stdbool.h>

// Funzione per il parsing di un JSON e la mappatura dei valori
void jsonMap(const char* json, void (*callback)(const char*, const char*, void*), void* data) {
    struct json_object* root = json_tokener_parse(json);
    enum json_type type = json_object_get_type(root);

    if (type == json_type_object) {
        json_object_object_foreach(root, key, val) {
            const char* value = json_object_get_string(val);
            callback(key, value, data);
        }
    }

    json_object_put(root);
}

// Funzione per il parsing di un JSON e la fold dei valori
void jsonFold(const char* json, void (*callback)(const char*, const char*, void*), void* accumulator) {
    struct json_object* root = json_tokener_parse(json);
    enum json_type type = json_object_get_type(root);

    if (type == json_type_object) {
        json_object_object_foreach(root, key, val) {
            const char* value = json_object_get_string(val);
            callback(key, value, accumulator);
        }
    }

    json_object_put(root);
}

// Funzione per formattare il risultato della query in JSON
char* formatQueryResultToJson(PGresult* result) {
    int rows = PQntuples(result);
    int columns = PQnfields(result);

    // Creazione dell'oggetto JSON principale
    json_object* json = json_object_new_array();

    // Iterazione sulle righe del risultato
    for (int i = 0; i < rows; i++) {
        // Creazione dell'oggetto JSON per ogni riga
        json_object* jsonRow = json_object_new_object();

        // Iterazione sulle colonne del risultato
        for (int j = 0; j < columns; j++) {
            // Ottenimento del nome della colonna
            const char* columnName = PQfname(result, j);

            // Ottenimento del valore della cella
            const char* cellValue = PQgetvalue(result, i, j);

            // Creazione dell'oggetto JSON per la colonna
            json_object* jsonColumn = json_object_new_string(cellValue);

            // Aggiunta della coppia nome-colonna/valore al JSON della riga
            json_object_object_add(jsonRow, columnName, jsonColumn);
        }

        // Aggiunta della riga al JSON principale
        json_object_array_add(json, jsonRow);
    }

    // Conversione del JSON in stringa
    const char* jsonString = json_object_to_json_string(json);
    char* formattedJsonString = strdup(jsonString);

    // Deallocazione delle risorse
    json_object_put(json);

    return formattedJsonString;
}


typedef struct {
    const char* desired_key;
    const char* desired_value;
    bool match_found;
} JSONCompareContext;

void compareKeyValueCallback(const char* key, const char* value, void* context) {
    JSONCompareContext* ctx = (JSONCompareContext*)context;
    if (strcmp(key, ctx->desired_key) == 0) {
        if (strcmp(value, ctx->desired_value) == 0) {
            ctx->match_found = true;
        }
    }
}

bool jsonCompare(const char* json, const char* desired_key, const char* desired_value) {
    JSONCompareContext context;
    context.desired_key = desired_key;
    context.desired_value = desired_value;
    context.match_found = false;

    jsonFold(json, compareKeyValueCallback, &context);

    return context.match_found;
}

void compareKeyCallback(const char* key, const char* value, void* context) {
    JSONCompareContext* ctx = (JSONCompareContext*)context;
    if (strcmp(key, ctx->desired_key) == 0) {
        ctx->desired_value = value;
        ctx->match_found = true;
    }
}

const char* getValueFromJson(const char* json, const char* key){
    JSONCompareContext context;
    context.desired_key = key;
    context.desired_value = NULL;
    context.match_found = false;

    jsonFold(json, compareKeyCallback, &context);

    return context.desired_value;
}
bool existsKeyInJson(const char* json,const char* key){
    return !(getValueFromJson(json,key) == NULL);
}


void print(const char* key, const char* value, void* data) {
    printf("%s: %s\n", key, value);
    fflush(stdout);
}

void printJsonKeysAndValues(const char* json) {
    printf("JSON PRINT:\n");
    fflush(stdout);
    jsonMap(json, print, NULL);
}

char** splitListJSON(const char* json, int* count) {
    struct json_object* root = json_tokener_parse(json);
    enum json_type type = json_object_get_type(root);

    if (type == json_type_array) {
        int arrLen = json_object_array_length(root);
        char** list = malloc(arrLen * sizeof(char*));
        *count = arrLen;

        for (int i = 0; i < arrLen; i++) {
            struct json_object* item = json_object_array_get_idx(root, i);
            const char* jsonString = json_object_to_json_string(item);
            list[i] = strdup(jsonString);
        }

        json_object_put(root);
        return list;
    }

    json_object_put(root);
    *count = 0;
    return NULL;
}

char* extractJSONListAsString(const char* json, const char* key) {
    struct json_object* root = json_tokener_parse(json);
    struct json_object* listObj;
    char* listString;

    if (json_object_object_get_ex(root, key, &listObj)) {
        listString = strdup(json_object_to_json_string(listObj));
    } else {
        listString = NULL;
    }

    json_object_put(root);
    return listString;
}

char** getListFromJson(const char* json, const char* key, int* out_count){
    *out_count = 0;
    char* jsonStrList = extractJSONListAsString(json,key);
    if(jsonStrList!=NULL){
        return splitListJSON(jsonStrList, out_count);
    }
    else return NULL;
}