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
#include "../../../models/DAOs/order/order_dao.h"


void getOrdersMadeByUserHandler(RouterParams params) {
    TokenPayload* token = decodeToken(params.request.authorization);
    if(token!= NULL){
        const char* decoded_email = token->email;
        int decoded_id = token->id;
        PGresult* orders_result = getOrdersMadeByUser(params.thread_data->connection,decoded_id);
        if(orders_result == NULL){
            const char *response = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
            send(params.thread_data->client_socket, response, strlen(response), 0);
            return;
        }
        char* json = formatQueryResultToJson(orders_result);
        // controlla che json non superi il buffer
        if( strlen(json) > 4096 ){
            const char *response = "HTTP/1.1 500 Server Error\r\nContent-Length: 8\r\n\r\ntoo many";
            send(params.thread_data->client_socket, response, strlen(response), 0);
            free(json);
            PQclear(orders_result);
            free(token);
            return;
        }
        char buffer[4096];
        HttpResponse response;
        response.code = "200 OK";
        response.contentType = "application/json";
        response.body = json;
        formatHttpResponse(buffer, sizeof(buffer), &response);
        send(params.thread_data->client_socket, buffer, strlen(buffer), 0);
        free(json);
        PQclear(orders_result);
        free(token);
    } else {
        const char *response = "HTTP/1.1 500 Server Error\r\nContent-Length: 0\r\n\r\n";
        send(params.thread_data->client_socket, response, strlen(response), 0);
    }
}

char* formatOrderResponse(Order* order, PGresult* order_drinks_result,float total){
    JsonProperty orderProps[] = {
        {"id", &order->id, INT}, 
        {"id_user", &order->id_user, INT},
        {"creation_datetime", (void*)order->creation_datetime, STRING},
        {"paid", &order->paid, BOOL}
    };
    int orderPropsCount = sizeof(orderProps) / sizeof(orderProps[0]);
    char* formattedOrder = formatJsonProps(orderProps, orderPropsCount);
     
    if(order_drinks_result == NULL){
        
        free(formattedOrder);
        return NULL;
    }
    char* jsonListDrinksOrdered = formatQueryResultToJson(order_drinks_result);
    char buffer[1024];
    HttpResponse response;
    response.code = "200 OK";
    response.contentType = "application/json";
    /*
    Formato
    {
        "drinks" : jsonListDrinksOrdered,
        "order" : formattedOrder
        "total" : total
    }
    */
    char* total_str = malloc(sizeof(char) * 10);
    snprintf(total_str, 10, "%f", total);
    const size_t dim = sizeof(char) * (strlen(jsonListDrinksOrdered) + strlen(formattedOrder) + strlen(total_str) + 50);
    char* body = malloc(dim);
    snprintf(body, dim, "{\"drinks\":%s,\"order\":%s,\"total\":%s}", jsonListDrinksOrdered, formattedOrder, total_str);
    response.body = body;
    formatHttpResponse(buffer, sizeof(buffer), &response);
    free(formattedOrder);
    free(jsonListDrinksOrdered);
    free(body);
    free(total_str);
    return strdup(buffer);
}
void getLastOrderMadeByUserHandler(RouterParams params){
    TokenPayload* token = decodeToken(params.request.authorization);
    if(token!= NULL){
        const char* decoded_email = token->email;
        int decoded_id = token->id;
        Order* order = getLastOrderMadeByUser(params.thread_data->connection,decoded_id);
        if(order == NULL){
            const char *response = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
            send(params.thread_data->client_socket, response, strlen(response), 0);
            return;
        } else {
            float total;
            PGresult* res = getOrderItemsByOrderId(params.thread_data->connection,order->id,&total);
            if(res == NULL){
                const char *response = "HTTP/1.1 500 Server Error\r\nContent-Length: 0\r\n\r\n";
                send(params.thread_data->client_socket, response, strlen(response), 0);
                free(order);
                free(token);
                return;
            }
            char* response = formatOrderResponse(order,res,total);
            
            send(params.thread_data->client_socket, response, strlen(response), 0);
            PQclear(res);
            free(order);
        }
        free(token);
    } else {
        const char *response = "HTTP/1.1 500 Server Error\r\nContent-Length: 0\r\n\r\n";
        send(params.thread_data->client_socket, response, strlen(response), 0);
    }
}

void getOrderHandler(RouterParams params){
    const char* str_order_id = getPathParameter(params.request.path);
    if(str_order_id == NULL){ 
        const char *response = "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n";
        send(params.thread_data->client_socket, response, strlen(response), 0);
        return;
    }
    TokenPayload* token = decodeToken(params.request.authorization);
    if(token!= NULL){
        const char* decoded_email = token->email;
        int decoded_id = token->id;
        Order* order = getOrderById(params.thread_data->connection,atoi(str_order_id));
        free(token);
        if (order == NULL){
            const char *response = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
            send(params.thread_data->client_socket, response, strlen(response), 0);
            return;
        } 
        if (order->id_user != decoded_id){
            const char *response = "HTTP/1.1 403 Forbidden\r\nContent-Length: 0\r\n\r\n";
            send(params.thread_data->client_socket, response, strlen(response), 0);
            free(order);
            return;
        }
        float total;
        PGresult* res = getOrderItemsByOrderId(params.thread_data->connection,order->id,&total);
        if(res == NULL){
            const char *response = "HTTP/1.1 500 Server Error\r\nContent-Length: 0\r\n\r\n";
            send(params.thread_data->client_socket, response, strlen(response), 0);
            free(order);
            free(token);
            return;
        }
        char* response = formatOrderResponse(order,res,total);
        
        send(params.thread_data->client_socket, response, strlen(response), 0);
        PQclear(res);
        free(order);
    } else {
        const char *response = "HTTP/1.1 500 Server Error\r\nContent-Length: 0\r\n\r\n";
        send(params.thread_data->client_socket, response, strlen(response), 0);
    }
}

void orderDrinkHandler(RouterParams params){
    if(!existsKeyInJson(params.request.body, "id_drink") ||
        !existsKeyInJson(params.request.body, "quantity")){
        const char *response = "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n";
        send(params.thread_data->client_socket, response, strlen(response), 0);
    }
    char* str_id_drink = getValueFromJson(params.request.body, "id_drink");
    int id_drink = atoi(str_id_drink);
    free(str_id_drink);
    char* str_quantity = getValueFromJson(params.request.body, "quantity");
    int quantity = atoi(str_quantity);
    free(str_quantity);
    printf("id_drink: %d, quantity: %d\n", id_drink, quantity);
    TokenPayload* token = decodeToken(params.request.authorization);
    if(token!= NULL){
        const char* decoded_email = token->email;
        int decoded_id = token->id;
        bool result = orderDrink(params.thread_data->connection, decoded_id, id_drink, quantity);
        if(result){
            const char *response = "HTTP/1.1 200 OK\r\nContent-Length: 0\r\n\r\n";
            send(params.thread_data->client_socket, response, strlen(response), 0);
        } else {
            const char *response = "HTTP/1.1 500 Server Error\r\nContent-Length: 0\r\n\r\n";
            send(params.thread_data->client_socket, response, strlen(response), 0);
        }
        free(token);
    } else {
        const char *response = "HTTP/1.1 500 Server Error\r\nContent-Length: 0\r\n\r\n";
        send(params.thread_data->client_socket, response, strlen(response), 0);
    }
        
}

void deleteDrinkFromOrderHandler(RouterParams params){
    TokenPayload* token = decodeToken(params.request.authorization);
    const char* std_id_drink = getPathParameter(params.request.path);
    if(std_id_drink == NULL){ 
        const char *response = "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n";
        send(params.thread_data->client_socket, response, strlen(response), 0);
        return;
    }
    if(token == NULL){
        const char *response = "HTTP/1.1 500 Server Error\r\nContent-Length: 0\r\n\r\n";
        send(params.thread_data->client_socket, response, strlen(response), 0);
        return;
    }
    bool deleted = deleteDrinkFromUnpaidOrder(params.thread_data->connection,token->id,atoi(std_id_drink));
    free(token);
    if(deleted){
        const char *response = "HTTP/1.1 200 OK\r\nContent-Length: 0\r\n\r\n";
        send(params.thread_data->client_socket, response, strlen(response), 0);
    } else {
        const char *response = "HTTP/1.1 500 Server Error\r\nContent-Length: 0\r\n\r\n";
        send(params.thread_data->client_socket, response, strlen(response), 0);
    }
}

void deleteOrderHandler(RouterParams params) {
    TokenPayload *token = decodeToken(params.request.authorization);
    if(token == NULL){
        const char *response = "HTTP/1.1 500 Server Error\r\nContent-Length: 0\r\n\r\n";
        send(params.thread_data->client_socket, response, strlen(response), 0);
        return;
    }
    bool deleted = deleteUnpaidOrder(params.thread_data->connection,token->id);
    free(token);
    if(deleted){
        const char *response = "HTTP/1.1 200 OK\r\nContent-Length: 0\r\n\r\n";
        send(params.thread_data->client_socket, response, strlen(response), 0);
    } else {
        const char *response = "HTTP/1.1 500 Server Error\r\nContent-Length: 0\r\n\r\n";
        send(params.thread_data->client_socket, response, strlen(response), 0);
    }
}

void updateDrinkQuantityInOrderHandler(RouterParams params) {
    TokenPayload *token = decodeToken(params.request.authorization);
    if(token == NULL){
        const char *response = "HTTP/1.1 500 Server Error\r\nContent-Length: 0\r\n\r\n";
        send(params.thread_data->client_socket, response, strlen(response), 0);
        return;
    }
    char* str_id_drink = getValueFromJson(params.request.body, "id_drink");
    char* str_quantity = getValueFromJson(params.request.body, "quantity");
    printf("str_id_drink: %s, str_quantity: %s\n", str_id_drink, str_quantity);
    int id_drink = atoi(str_id_drink);
    int quantity = atoi(str_quantity);
    printf("id_drink: %d, quantity: %d\n", id_drink, quantity);
    bool updated = updateDrinkQuantityFromUnpaidOrder(params.thread_data->connection,token->id,id_drink,quantity);
    free(str_quantity);
    free(str_id_drink);
    free(token);
    if(updated){
        const char *response = "HTTP/1.1 200 OK\r\nContent-Length: 0\r\n\r\n";
        send(params.thread_data->client_socket, response, strlen(response), 0);
    } else {
        const char *response = "HTTP/1.1 500 Server Error\r\nContent-Length: 0\r\n\r\n";
        send(params.thread_data->client_socket, response, strlen(response), 0);
    }
}