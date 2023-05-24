#ifndef HANDLERS_H
#define HANDLERS_H
#include "../router/router.h"

void homeHandler(RouterParams params);
void loginHandler(RouterParams params);
void registerHandler(RouterParams params);
void getUserHandler(RouterParams params);
void getDrinkImageHandler(RouterParams params);
void getDrinkHandler(RouterParams params);
void getDrinksHandler(RouterParams params);
void getOrdersMadeByUserHandler(RouterParams params);
void getLastOrderMadeByUserHandler(RouterParams params);
void getOrderHandler(RouterParams params);
void orderDrinkHandler(RouterParams params);
void deleteDrinkFromOrderHandler(RouterParams params);
void deleteOrderHandler(RouterParams params);
#endif