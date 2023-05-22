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

// User methods

User* getUserById(int user_id);
User* getUserByEmail(const char* user_email);
void insertUser(User* user);

// Drink methods

Drink* getDrinkById(int drink_id);
void insertDrink(Drink* drink);
Drink** getDrinks();

// Order methods

Order* getOrderById(int order_id);
void insertOrder(Order* order);
Order** getOrdersMadeByEmail(const char* user_email);

// OrderItem methods

OrderItem* getOrderItemById(int order_item_id);
void insertOrderItem(OrderItem* order_item);

// Payment methods

Payment* getPaymentById(int payment_id);
void insertPayment(Payment* payment);
Payment** getPaymentsMadeByEmail(const char* user_email);


#endif