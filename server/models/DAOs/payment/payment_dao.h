#ifndef PAYMENT_DAO_H
#define PAYMENT_DAO_H
bool payUnpaidOrder(PGconn* connection, Payment new_payment);
#endif