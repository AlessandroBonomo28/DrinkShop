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


#endif