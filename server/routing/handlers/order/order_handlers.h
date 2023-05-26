#ifndef ORDER_HANDLERS_H
#define ORDER_HANDLERS_H
#include "../../router/router.h"

void getOrdersMadeByUserHandler(RouterParams params);
void getLastOrderMadeByUserHandler(RouterParams params);
void getOrderHandler(RouterParams params);
void orderDrinkHandler(RouterParams params);
void deleteDrinkFromOrderHandler(RouterParams params);
void deleteOrderHandler(RouterParams params);
void updateDrinkQuantityInOrderHandler(RouterParams params);
#endif