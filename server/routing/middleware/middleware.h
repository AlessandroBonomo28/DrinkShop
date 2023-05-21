#ifndef MIDDLEWARE_H
#define MIDDLEWARE_H
#include "../router/router.h"
void requiresAuth(RequestParams params, void (*next)(RequestParams params));

#endif