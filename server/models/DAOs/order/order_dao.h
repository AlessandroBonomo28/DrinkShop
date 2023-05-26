#ifndef ORDER_DAO_H
#define ORDER_DAO_H

PGresult* getOrdersMadeByUser(PGconn* connection,int id);
Order* getLastOrderMadeByUser(PGconn* connection, int id);
PGresult* getOrderItemsByOrderId(PGconn* connection, int id, float *totalAmount);
Order* getOrderById(PGconn* connection, int id);
bool orderDrink(PGconn* connection, int id_user, int id_drink, int quantity);
bool deleteDrinkFromUnpaidOrder(PGconn* connection,int id_user, int id_drink);
bool deleteUnpaidOrder(PGconn* connection, int id_user);
bool updateDrinkQuantityFromUnpaidOrder(PGconn* connection,int id_user, int id_drink, int new_quantity);

#endif