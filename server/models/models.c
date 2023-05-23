// includi libreria postgres
#include <postgresql/libpq-fe.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "models.h"

User* authenticateUser(PGconn* connection, const char* email, const char* password) {
    const char* query = "SELECT * FROM \"Users\" WHERE \"email\" = $1 AND \"password\" = $2;";
    const char* param_values[2];
    param_values[0] = email;
    param_values[1] = password;
    const int param_lengths[2] = { strlen(email), strlen(password) };
    const int param_formats[2] = { 0, 0 };

    PGresult* result = PQexecParams(connection, query, 2, NULL, param_values, param_lengths, param_formats, 0);

    if (PQresultStatus(result) == PGRES_TUPLES_OK) {
        int rows = PQntuples(result);
        if (rows == 0) {
            return NULL;
        }
        
        User* user = malloc(sizeof(User));
        user->id = atoi(PQgetvalue(result, 0, 0));
        user->password = PQgetvalue(result, 0, 1);
        user->email = PQgetvalue(result, 0, 2);
        return user;
    } else {
        return NULL; // error
    }
}

User* registerUser(PGconn* connection, const char* email, const char* password) {
    const char* query = "INSERT INTO \"Users\" (\"email\", \"password\") VALUES ($1, $2) RETURNING *;";
    const char* param_values[2];
    param_values[0] = email;
    param_values[1] = password;
    const int param_lengths[2] = { strlen(email), strlen(password) };
    const int param_formats[2] = { 0, 0 };

    PGresult* result = PQexecParams(connection, query, 2, NULL, param_values, param_lengths, param_formats, 0);

    if (PQresultStatus(result) == PGRES_TUPLES_OK) {
        int rows = PQntuples(result);
        if (rows == 0) {
            return NULL;
        }
        
        User* user = malloc(sizeof(User));
        user->id = atoi(PQgetvalue(result, 0, 0));
        user->email = PQgetvalue(result, 0, 1);
        user->password = PQgetvalue(result, 0, 2);
        return user;
    } else {
        return NULL; // error
    }
}

User* getUserById(PGconn* connection, int id) {
    const char* query = "SELECT * FROM \"Users\" WHERE \"id\" = $1;";
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
        
        User* user = malloc(sizeof(User));
        user->id = atoi(PQgetvalue(result, 0, 0));
        user->password = PQgetvalue(result, 0, 1);
        user->email = PQgetvalue(result, 0, 2);
        return user;
    } else {
        return NULL; // error
    }
}

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
        drink->id = atoi(PQgetvalue(result, 0, 0));
        drink->name = PQgetvalue(result, 0, 1);
        drink->description = PQgetvalue(result, 0, 2);
        drink->image_url = PQgetvalue(result, 0, 3);
        drink->price = atof(PQgetvalue(result, 0, 4));
        return drink;
    } else {
        return NULL; // error
    }
}

PGresult* getDrinks(PGconn* connection) {
    const char* query = "SELECT * FROM \"Drinks\" ORDER BY \"id\" ASC;";
    PGresult* result = PQexec(connection, query);
    return result;
}

PGresult* getOrdersMadeByUser(PGconn* connection,int id) {
    const char* query = "SELECT * FROM \"Orders\" WHERE \"user_id\" = $1 ORDER BY \"id\" ASC;";
    const char* param_values[1];
    char id_str[10];
    sprintf(id_str, "%d", id);
    param_values[0] = id_str;
    const int param_lengths[1] = { strlen(id_str) };
    const int param_formats[1] = { 0 };

    PGresult* result = PQexecParams(connection, query, 1, NULL, param_values, param_lengths, param_formats, 0);
    return result;
}

Order* getLastOrderMadeByUser(PGconn* connection, int id) {
    const char* query = "SELECT * FROM \"Orders\" WHERE \"id_user\" = $1 ORDER BY \"id\" DESC LIMIT 1;";
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
        
        Order* order = malloc(sizeof(Order));
        order->id = atoi(PQgetvalue(result, 0, 0));
        order->id_user = atoi(PQgetvalue(result, 0, 1));
        order->creation_datetime = PQgetvalue(result, 0, 2);
        order->paid = (strcmp(PQgetvalue(result, 0, 3),"t") == 0 ? true : false);
        return order;
    } else {
        return NULL; // error
    }
}

PGresult* getOrderItemsByOrderId(PGconn* connection, int id) {
    const char* query = "SELECT * FROM \"OrderItems\" WHERE \"id_order\" = $1;";
    const char* param_values[1];
    char id_str[10];
    sprintf(id_str, "%d", id);
    param_values[0] = id_str;
    const int param_lengths[1] = { strlen(id_str) };
    const int param_formats[1] = { 0 };

    PGresult* result = PQexecParams(connection, query, 1, NULL, param_values, param_lengths, param_formats, 0);
    return result;
}