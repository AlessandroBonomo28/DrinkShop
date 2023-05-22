#ifndef HANDLERS_H
#define HANDLERS_H
#include "../router/router.h"

void requiresAuth(HandlerParams params, void (*next)(HandlerParams params));

void homeHandler(HandlerParams params);
void loginHandler(HandlerParams params);
void registerHandler(HandlerParams params);
void sayHello(HandlerParams params);
#endif