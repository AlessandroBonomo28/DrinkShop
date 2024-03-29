#include <postgresql/libpq-fe.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../../models.h"

PGresult* getOrdersMadeByUser(PGconn* connection,int id) {
    // bisogna selezionare in questo modo per evitare che gli attributi 
    // di due tabelle diverse abbiano lo stesso nome
    const char* query = (
        "SELECT \"Orders\".*, \"Payments\".\"id\" AS id_payment, \
        \"Payments\".\"card_holder\", \"Payments\".\"card_number\", \
        \"Payments\".\"CVV\", \"Payments\".\"expiration_date\", \
        \"Payments\".\"creation_timestamp\" as payment_timestamp, \"Payments\".\"amount\" \
        FROM \"Orders\" \
        FULL JOIN \"Payments\" ON \"Orders\".\"id\" = \"Payments\".\"id_order\" \
        WHERE \"Orders\".\"id_user\" = $1 \
        ORDER BY \"Orders\".\"id\" DESC LIMIT 10;"
    );
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
        return NULL; 
    }
}

PGresult* getOrderItemsByOrderId(PGconn* connection, int id, float *totalAmount) {
    // bisogna selezionare in questo modo per evitare di avere attributi con lo stesso nome
    const char* query = (
        "SELECT \"OrderItems\".*, \"Drinks\".\"id\" as \"id_drink\", \"Drinks\".\"name\", \
         \"Drinks\".\"description\", \"Drinks\".\"image_url\", \"Drinks\".\"price\" , \"Drinks\".\"drink_type\" \
        FROM \"OrderItems\" \
        INNER JOIN \"Drinks\" ON \"OrderItems\".\"id_item\" = \"Drinks\".\"id\" \
        WHERE \"id_order\" = $1;"

    );
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
        return NULL; 
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

bool deleteDrinkFromUnpaidOrder(PGconn* connection,int id_user, int id_drink){
    const char* query = "SELECT delete_drink_from_unpaid_order($1,$2);";
    const char* param_values[2];
    char id_user_str[10];
    sprintf(id_user_str, "%d", id_user);
    param_values[0] = id_user_str;
    char id_drink_str[10];
    sprintf(id_drink_str, "%d", id_drink);
    param_values[1] = id_drink_str;
    const int param_lengths[2] = { strlen(id_user_str), strlen(id_drink_str) };
    const int param_formats[2] = { 0, 0 };

    PGresult* result = PQexecParams(connection, query, 2, NULL, param_values, param_lengths, param_formats, 0);
    if(PQresultStatus(result) == PGRES_TUPLES_OK){
        PQclear(result);
        return true;
    } else{
        PQclear(result);
        return false;
    }
}

bool deleteUnpaidOrder(PGconn* connection, int id_user){
    const char* query = "SELECT delete_unpaid_order($1);";
    const char* param_values[1];
    char id_user_str[10];
    sprintf(id_user_str, "%d", id_user);
    param_values[0] = id_user_str;
    const int param_lengths[1] = { strlen(id_user_str) };
    const int param_formats[1] = { 0 };

    PGresult* result = PQexecParams(connection, query, 1, NULL, param_values, param_lengths, param_formats, 0);
    if(PQresultStatus(result) == PGRES_TUPLES_OK){
        PQclear(result);
        return true;
    } else{
        PQclear(result);
        return false;
    }
}

bool updateDrinkQuantityFromUnpaidOrder(PGconn* connection,int id_user, int id_drink, int new_quantity) {
    const char* query = "SELECT update_drink_quantity_from_unpaid_order($1,$2,$3);";
    const char* param_values[3];
    char id_user_str[10];
    sprintf(id_user_str, "%d", id_user);
    param_values[0] = id_user_str;
    char id_drink_str[10];
    sprintf(id_drink_str, "%d", id_drink);
    param_values[1] = id_drink_str;
    char new_quantity_str[10];
    sprintf(new_quantity_str, "%d", new_quantity);
    param_values[2] = new_quantity_str;
    const int param_lengths[3] = { strlen(id_user_str), strlen(id_drink_str), strlen(new_quantity_str) };
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