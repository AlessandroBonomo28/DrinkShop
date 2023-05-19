#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <postgresql/libpq-fe.h>
#include <json-c/json.h>

// Funzione per il parsing di un JSON e la mappatura dei valori
void parseJSONMap(const char* json, void (*callback)(const char*, const char*, void*), void* data) {
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
void parseJSONFold(const char* json, void (*callback)(const char*, const char*, void*), void* accumulator) {
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
