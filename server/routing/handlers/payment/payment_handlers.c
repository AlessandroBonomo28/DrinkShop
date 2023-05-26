#include <string.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <postgresql/libpq-fe.h>
#include "../../../utils/json_helper/json_helper.h"
#include "../../router/router.h"
#include "../../../utils/jwt_helper/jwt_helper.h"
#include "../../../utils/http_helper/http_helper.h"
#include "../../../utils/file_helper/file_helper.h"
#include "../../../models/models.h"
#include "../../../utils/crypt_helper/crypt_helper.h"
#include "../../../utils/datetime_helper/datetime_helper.h"
#include "../../../models/DAOs/payment/payment_DAO.h"

void payOrderHandler(RouterParams params) {
    TokenPayload *token = decodeToken(params.request.authorization);
    if(token == NULL){
        const char *response = "HTTP/1.1 500 Server Error\r\nContent-Length: 0\r\n\r\n";
        send(params.thread_data->client_socket, response, strlen(response), 0);
        return;
    }
    char* str_amount = getValueFromJson(params.request.body, "amount");
    printf("amount str: %s\n",str_amount);
    char* str_card_holder = getValueFromJson(params.request.body, "card_holder");
    char* str_card_number = getValueFromJson(params.request.body, "card_number");
    char* str_CVV = getValueFromJson(params.request.body, "CVV");
    char* str_expiration_date = getValueFromJson(params.request.body, "expiration_date");
    // se almeno uno dei valori è null allora errore 404
    if(str_amount == NULL || str_card_holder == NULL || str_card_number == NULL ||
       str_CVV == NULL || str_expiration_date == NULL){
        const char *response = "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n";
        send(params.thread_data->client_socket, response, strlen(response), 0);
        free(str_amount);
        free(str_card_holder);
        free(str_card_number);
        free(str_CVV);
        free(str_expiration_date);
        free(token);
        return;
    }
    float amount = atof(str_amount);
    printf("amount: %f\n",amount);
    Payment new_payment;
    new_payment.id_user = token->id;
    new_payment.card_holder = str_card_holder;
    new_payment.card_number = str_card_number;
    new_payment.CVV = str_CVV;
    new_payment.expiration_date = str_expiration_date;
    new_payment.amount = amount;
    bool paid = payUnpaidOrder(params.thread_data->connection,new_payment);
    free(str_card_holder);
    free(str_card_number);
    free(str_CVV);
    free(str_expiration_date);
    free(str_amount);
    free(token);
    if(paid){
        const char *response = "HTTP/1.1 200 OK\r\nContent-Length: 0\r\n\r\n";
        send(params.thread_data->client_socket, response, strlen(response), 0);
    } else {
        const char *response = "HTTP/1.1 500 Server Error\r\nContent-Length: 0\r\n\r\n";
        send(params.thread_data->client_socket, response, strlen(response), 0);
    }
}