#include <postgresql/libpq-fe.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../../models.h"

Drink* getDrinkById(PGconn* connection, int id) {
    const char* query = "SELECT * FROM \"Drinks\" WHERE \"id\" = $1;";
    const char* param_values[1];
    char id_str[10];
    sprintf(id_str, "%d", id);
    param_values[0] = id_str;
    const int param_lengths[1] = { strlen(id_str) };
    const int param_formats[1] = { 0 };

    PGresult* result = PQexecParams(connection, query, 1, NULL, param_values, param_lengths, param_formats, 0);

    if (PQresultStatus(result) == PGRES_TUPLES_OK) {
        int rows = PQntuples(result);
        if (rows == 0) {
            return NULL;
        }
        
        Drink* drink = malloc(sizeof(Drink));
        drink->id = atoi(strdup(PQgetvalue(result, 0, 0)));
        drink->name = strdup(PQgetvalue(result, 0, 1));
        drink->description = strdup(PQgetvalue(result, 0, 2));
        drink->image_url = strdup(PQgetvalue(result, 0, 3));
        drink->price = atof(strdup(PQgetvalue(result, 0, 4)));
        PQclear(result);
        return drink;
    } else {
        return NULL; 
    }
}

PGresult* getDrinks(PGconn* connection) {
    const char* query = "SELECT * FROM \"Drinks\" ORDER BY \"id\" ASC;";
    PGresult* result = PQexec(connection, query);
    return result;
}