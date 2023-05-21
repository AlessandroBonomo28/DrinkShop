#ifndef HANDLERS_H
#define HANDLERS_H
#include "../router/router.h"

void requiresAuth(RequestParams params, void (*next)(RequestParams params));

void homeHandler(RequestParams params);
void loginHandler(RequestParams params);
void registerHandler(RequestParams params);
void sayHello(RequestParams params);
#endif