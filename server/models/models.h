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
    const char* creation_datetime;
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
    const char* card_holder;
    const char* card_number;
    const char* CVV;
    const char* expiration_date;
    const char* creation_datetime;
    float amount;
} Payment;

User* authenticateUser(PGconn* connection, const char* email, const char* password);
User* registerUser(PGconn* connection, const char* email, const char* password);
User* getUserById(PGconn* connection, int id);
User* getUserByEmail(PGconn* connection, const char* email);

Drink* getDrinkById(PGconn* connection, int id);
PGresult* getDrinks(PGconn* connection);
PGresult* getOrdersMadeByUser(PGconn* connection,int id);
Order* getLastOrderMadeByUser(PGconn* connection, int id);
PGresult* getOrderItemsByOrderId(PGconn* connection, int id, float *totalAmount);
Order* getOrderById(PGconn* connection, int id);
bool orderDrink(PGconn* connection, int id_user, int id_drink, int quantity);
bool deleteDrinkFromOrder(PGconn* connection, int id_drink);
bool deleteUnpaidOrder(PGconn* connection, int id_user);
#endif