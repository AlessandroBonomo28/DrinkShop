#ifndef MIDDLEWARE_H
#define MIDDLEWARE_H
#include "../router/router.h"
void requiresAuth(HandlerParams params, void (*next)(HandlerParams params));

#endif