#ifndef MODELS_H
#define MODELS_H

typedef struct {
    int id;
    const char* email;
    const char* password;
} User;

typedef struct {
    int id;
    const char* name;
    const char* description;
    const char* image_url;
    float price;
} Drink;

typedef struct {
    int id;
    int id_user;
    const char* creation_datetime; // TODO date type
    bool paid;
} Order;

typedef struct {
    int id;
    int id_order;
    int id_item;
    int quantity;
} OrderItem;

typedef struct {
    int id;
    int id_order;
    int id_user;
    const char* provider;
    const char* creation_datetime; // TODO date type
    float amount;
} Payment;

User* authenticateUser(PGconn* connection, const char* email, const char* password);
User* registerUser(PGconn* connection, const char* email, const char* password);
User* getUserById(PGconn* connection, int id);

Drink* getDrinkById(PGconn* connection, int id);
PGresult* getDrinks(PGconn* connection);
PGresult* getOrdersMadeByUser(PGconn* connection,int id);
Order* getLastOrderMadeByUser(PGconn* connection, int id);
PGresult* getOrderItemsByOrderId(PGconn* connection, int id);
#endif