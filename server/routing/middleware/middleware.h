#ifndef MIDDLEWARE_H
#define MIDDLEWARE_H
#include "../router/router.h"
void requiresAuth(RouterParams params, void (*next)(RouterParams params));

#endif