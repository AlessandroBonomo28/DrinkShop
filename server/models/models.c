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
        user->id = atoi(strdup(PQgetvalue(result, 0, 0)));
        user->password = strdup(PQgetvalue(result, 0, 1));
        user->email = strdup(PQgetvalue(result, 0, 2));
        PQclear(result);
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
        user->id = atoi(strdup(PQgetvalue(result, 0, 0)));
        user->email = strdup(PQgetvalue(result, 0, 1));
        user->password = strdup(PQgetvalue(result, 0, 2));
        PQclear(result);
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
        user->id = atoi(strdup(PQgetvalue(result, 0, 0)));
        user->password = strdup(PQgetvalue(result, 0, 1));
        user->email = strdup(PQgetvalue(result, 0, 2));
        PQclear(result);
        return user;
    } else {
        return NULL; // error
    }
}

User* getUserByEmail(PGconn* connection, const char* email) {
    const char* query = "SELECT * FROM \"Users\" WHERE \"email\" = $1;";
    const char* param_values[1];
    param_values[0] = email;
    const int param_lengths[1] = { strlen(email) };
    const int param_formats[1] = { 0 };

    PGresult* result = PQexecParams(connection, query, 1, NULL, param_values, param_lengths, param_formats, 0);

    if (PQresultStatus(result) == PGRES_TUPLES_OK) {
        int rows = PQntuples(result);
        if (rows == 0) {
            return NULL;
        }
        
        User* user = malloc(sizeof(User));
        user->id = atoi(strdup(PQgetvalue(result, 0, 0)));
        user->password = strdup(PQgetvalue(result, 0, 1));
        user->email = strdup(PQgetvalue(result, 0, 2));
        PQclear(result);
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
        drink->id = atoi(strdup(PQgetvalue(result, 0, 0)));
        drink->name = strdup(PQgetvalue(result, 0, 1));
        drink->description = strdup(PQgetvalue(result, 0, 2));
        drink->image_url = strdup(PQgetvalue(result, 0, 3));
        drink->price = atof(strdup(PQgetvalue(result, 0, 4)));
        PQclear(result);
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
    const char* query = "SELECT * FROM \"Orders\" WHERE \"id_user\" = $1 ORDER BY \"id\" ASC;";
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
        order->id = atoi(strdup(PQgetvalue(result, 0, 0)));
        order->id_user = atoi(strdup(PQgetvalue(result, 0, 1)));
        order->creation_datetime = strdup(PQgetvalue(result, 0, 2));
        order->paid = (strcmp(PQgetvalue(result, 0, 3),"t") == 0 ? true : false);
        PQclear(result);
        return order;
    } else {
        return NULL; // error
    }
}

PGresult* getOrderItemsByOrderId(PGconn* connection, int id, float *totalAmount) {
    const char* query = "SELECT * FROM \"OrderItems\" INNER JOIN \"Drinks\" ON \"OrderItems\".\"id_item\" = \"Drinks\".\"id\" WHERE \"id_order\" = $1;";
    const char* param_values[1];
    char id_str[10];
    sprintf(id_str, "%d", id);
    param_values[0] = id_str;
    const int param_lengths[1] = { strlen(id_str) };
    const int param_formats[1] = { 0 };

    PGresult* result = PQexecParams(connection, query, 1, NULL, param_values, param_lengths, param_formats, 0);
    *totalAmount = 0;
    for (int i = 0; i < PQntuples(result); i++) {
        //printf("price = %s\n", PQgetvalue(result, i, 8));
        //printf("quantity = %s\n", PQgetvalue(result, i, 3));
        *totalAmount += atoi(PQgetvalue(result,i,3))*atof(PQgetvalue(result, i, 8));
    }
    return result;
}

Order* getOrderById(PGconn* connection, int id) {
    const char* query = "SELECT * FROM \"Orders\" WHERE \"id\" = $1;";
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
        order->id = atoi(strdup(PQgetvalue(result, 0, 0)));
        order->id_user = atoi(strdup(PQgetvalue(result, 0, 1)));
        order->creation_datetime = strdup(PQgetvalue(result, 0, 2));
        order->paid = (strcmp(PQgetvalue(result, 0, 3),"t") == 0 ? true : false);
        PQclear(result);
        return order;
    } else {
        return NULL; // error
    }
}
bool orderDrink(PGconn* connection, int id_user, int id_drink, int quantity){
    const char* query = "SELECT order_drink($1,$2,$3);";
    const char* param_values[3];
    char id_user_str[10];
    sprintf(id_user_str, "%d", id_user);
    param_values[0] = id_user_str;
    char id_drink_str[10];
    sprintf(id_drink_str, "%d", id_drink);
    param_values[1] = id_drink_str;
    char quantity_str[10];
    sprintf(quantity_str, "%d", quantity);
    param_values[2] = quantity_str;
    const int param_lengths[3] = { strlen(id_user_str), strlen(id_drink_str), strlen(quantity_str) };
    const int param_formats[3] = { 0, 0, 0 };

    PGresult* result = PQexecParams(connection, query, 3, NULL, param_values, param_lengths, param_formats, 0);
    if(PQresultStatus(result) == PGRES_TUPLES_OK){
        PQclear(result);
        return true;
    } else{
        PQclear(result);
        return false;
    }
}