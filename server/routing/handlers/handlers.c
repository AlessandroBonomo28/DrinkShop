#include <string.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <postgresql/libpq-fe.h>
#include "../../utils/json_helper/json_helper.h"
#include "../router/router.h"
#include "../../utils/jwt_helper/jwt_helper.h"
#include "../../utils/http_helper/http_helper.h"
#include "../../utils/file_helper/file_helper.h"
#include "../../models/models.h"
#include "../../utils/crypt_helper/crypt_helper.h"
//TODO sistemare il body delle risposte http
void homeHandler(RouterParams params) {
    TokenPayload* token = decodeToken(params.request.authorization);
    if(token!= NULL){
        const char* decoded_email = token->email;
        int decoded_id = token->id;
        char buffer[1024];
        char body[100];
        snprintf(body, sizeof(body),"Hello, %s. Your id:%d",decoded_email,decoded_id);
        HttpResponse response;
        response.code = "200 OK";
        response.contentType = "application/json";
        response.body = body;
        formatHttpResponse(buffer, sizeof(buffer), &response);
        send(params.thread_data->client_socket, buffer, strlen(buffer), 0);
        free(token);
    } else {
        const char *response = "HTTP/1.1 500 Server Error\r\nContent-Length: 0\r\n\r\n";
        send(params.thread_data->client_socket, response, strlen(response), 0);
    }
}

void loginHandler(RouterParams params) {
    char *response = "HTTP/1.1 401 Unauthorized\r\nContent-Length: 15\r\n\r\nNot Authorized!";
    char* email = getValueFromJson(params.request.body,"email");
    char* plain_pw = getValueFromJson(params.request.body,"password");
    if(email == NULL || plain_pw == NULL){
        free(email);
        free(plain_pw);
        // 400 Bad Request
        response = "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n";
        send(params.thread_data->client_socket, response, strlen(response), 0);
        return;
    }
    char* pw = encrypt(plain_pw);
    free(plain_pw);
    User* user = authenticateUser(params.thread_data->connection,email,pw);
    free(email);
    free(pw);
    if(user!=NULL){
        TokenPayload payload;
        payload.email = user->email;
        payload.id = user->id;
        free(user);
        char* token = encodeToken(&payload);
        if (token == NULL) {
            response = "HTTP/1.1 500 Internal Server Error\r\nContent-Length: 23\r\n\r\nToken generation failed!";
        } else {
            // Creazione del JSON di risposta
            JsonProperty props[] = {
                {"token", (void*)token, STRING}
            };
            char* jsonValue = formatJsonProps(props,1);

            // Creazione della risposta HTTP includendo il JSON formattato
            char responseBuffer[1024];
            snprintf(responseBuffer, sizeof(responseBuffer),
                "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: %zu\r\n\r\n%s",
                strlen(jsonValue),
                jsonValue);

            send(params.thread_data->client_socket, responseBuffer, strlen(responseBuffer), 0);

            free(jsonValue);
            free(token);
            return;
        }
    }
    send(params.thread_data->client_socket, response, strlen(response), 0);
}

void registerHandler(RouterParams params) {
    char* email = getValueFromJson(params.request.body,"email");
    char* plain_pw = getValueFromJson(params.request.body,"password");
    if(email == NULL || plain_pw == NULL){
        free(email);
        free(plain_pw);
        // 400 Bad Request
        const char *response = "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n";
        send(params.thread_data->client_socket, response, strlen(response), 0);
        return;
    }
    char* pw = encrypt(plain_pw);
    free(plain_pw);
    User* user = registerUser(params.thread_data->connection,email,pw);
    free(email);
    free(pw);
    if(user!=NULL){
        const char *response = "HTTP/1.1 200 OK\r\nContent-Length: 0\r\n\r\n";
        send(params.thread_data->client_socket, response, strlen(response), 0);
        free(user);
        return;
    }else {
        // 500 Internal Server Error
        const char *response = "HTTP/1.1 500 Internal Server Error\r\nContent-Length: 0\r\n\r\n";
        send(params.thread_data->client_socket, response, strlen(response), 0);
    }
}

void getUserHandler(RouterParams params) {
    const char* str_id = getPathParameter(params.request.path);
    if(str_id == NULL){
        // 400 Bad Request
        const char *response = "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n";
        send(params.thread_data->client_socket, response, strlen(response), 0);
        return;
    }
    User* user = getUserById(params.thread_data->connection,atoi(str_id));
    if(user != NULL){
        JsonProperty props[] = {
            {"email", (void*)user->email, STRING}, 
            //{"password", (void*)user->password, STRING},
            {"id", &user->id, INT}
            //{"float example", &val, FLOAT},
            //{"bool example", &boolValue, BOOL},
            //{"static string example","hello static",STRING}
        };

        int propsCount = sizeof(props) / sizeof(props[0]);
        char* formattedJson = formatJsonProps(props, propsCount);
        char response[1024];
        snprintf(response, sizeof(response),
            "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: %zu\r\n\r\n%s",
            strlen(formattedJson),
            formattedJson);
        send(params.thread_data->client_socket, response, strlen(response), 0);
        free(user);
        free(formattedJson);
        return;
    } else {
        const char *response = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
        send(params.thread_data->client_socket, response, strlen(response), 0);
    }
}

void getDrinkImageHandler(RouterParams params) {
    // ottieni il parametro id dalla path
    const char* str_drink_id = getPathParameter(params.request.path);
    if(str_drink_id == NULL){ 
        const char *response = "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n";
        send(params.thread_data->client_socket, response, strlen(response), 0);
        return;
    }
    Drink* drink = getDrinkById(params.thread_data->connection,atoi(str_drink_id));
    if(drink == NULL){
        // 404 Drink not found
        const char *response = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
        send(params.thread_data->client_socket, response, strlen(response), 0);
        return;
    }
    const char* path = drink->image_url;
    if(fileExists(path))
        serveFile(path,params.thread_data->client_socket);
    else {
        const char *response = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
        send(params.thread_data->client_socket, response, strlen(response), 0);
    }
    free(drink);
}

void getDrinkHandler(RouterParams params) {
    const char* str_drink_id = getPathParameter(params.request.path);
    if(str_drink_id == NULL){ 
        const char *response = "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n";
        send(params.thread_data->client_socket, response, strlen(response), 0);
        return;
    }
    Drink* drink = getDrinkById(params.thread_data->connection,atoi(str_drink_id));
    if(drink == NULL){
        // 404 Drink not found
        const char *response = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
        send(params.thread_data->client_socket, response, strlen(response), 0);
        return;
    }
    JsonProperty props[] = {
        {"name", (void*)drink->name, STRING}, 
        {"description", (void*)drink->description, STRING},
        {"price", &drink->price, FLOAT},
        {"id", &drink->id, INT},
        {"image_url", (void*)drink->image_url, STRING}
    };

    int propsCount = sizeof(props) / sizeof(props[0]);
    char* formattedJson = formatJsonProps(props, propsCount);
    char response[1024];
    snprintf(response, sizeof(response),
        "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: %zu\r\n\r\n%s",
        strlen(formattedJson),
        formattedJson);
    send(params.thread_data->client_socket, response, strlen(response), 0);
    free(drink);
    free(formattedJson);
}

void getDrinksHandler(RouterParams params) {
    PGresult* drinks_result = getDrinks(params.thread_data->connection);
    if(drinks_result == NULL){
        // 404 Drink not found
        const char *response = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
        send(params.thread_data->client_socket, response, strlen(response), 0);
        return;
    }
    char* json = formatQueryResultToJson(drinks_result);
    char buffer[1024];
    HttpResponse response;
    response.code = "200 OK";
    response.contentType = "application/json";
    response.body = json;
    formatHttpResponse(buffer, sizeof(buffer), &response);
    send(params.thread_data->client_socket, buffer, strlen(buffer), 0);
    free(json);
    PQclear(drinks_result);
}

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
        char buffer[1024];
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
/*
json request:
{
    "id_drink": 1,
    "quantity": 2
}
*/
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