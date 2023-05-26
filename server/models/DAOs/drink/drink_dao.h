#ifndef DRINK_DAO_H
#define DRINK_DAO_H

Drink* getDrinkById(PGconn* connection, int id);
PGresult* getDrinks(PGconn* connection);

#endif