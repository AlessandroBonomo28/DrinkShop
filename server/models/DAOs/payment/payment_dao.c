#include <postgresql/libpq-fe.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../../models.h"

bool payUnpaidOrder(PGconn* connection, Payment new_payment) {
    const char* query = "SELECT pay_unpaid_order($1,$2,$3,$4,$5,$6);";
    const char* param_values[6];
    char id_user_str[10];
    sprintf(id_user_str, "%d", new_payment.id_user);
    param_values[0] = id_user_str;
    param_values[1] = new_payment.card_holder;
    param_values[2] = new_payment.card_number;
    param_values[3] = new_payment.CVV;
    param_values[4] = new_payment.expiration_date;
    char amount_str[50];
    sprintf(amount_str, "%f", new_payment.amount);
    param_values[5] = amount_str;
    const int param_lengths[6] = { 
        strlen(id_user_str), 
        strlen(new_payment.card_holder),
        strlen(new_payment.card_number), 
        strlen(new_payment.CVV), 
        strlen(new_payment.expiration_date), 
        strlen(amount_str)
    };
    const int param_formats[6] = { 0, 0, 0, 0, 0, 0 };

    PGresult* result = PQexecParams(connection, query, 6, NULL, param_values, param_lengths, param_formats, 0);
    if(PQresultStatus(result) == PGRES_TUPLES_OK){
        PQclear(result);
        return true;
    } else{
        PQclear(result);
        return false;
    }
}