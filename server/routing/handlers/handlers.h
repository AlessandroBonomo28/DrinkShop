#ifndef HANDLERS_H
#define HANDLERS_H
#include "../router/router.h"

void requiresAuth(RouterParams params, void (*next)(RouterParams params));

void homeHandler(RouterParams params);
void loginHandler(RouterParams params);
void registerHandler(RouterParams params);
void sayHello(RouterParams params);
#endif