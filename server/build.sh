#!/bin/bash

# ljson = libreria json
# lpq = libreria di postgres
# ljwt = libreria jwt token
# Compila il server
gcc -o server \
    server.c \
    routing/router/router.c \
    routing/handlers/basic_handlers.c \
    routing/handlers/user/user_handlers.c \
    routing/handlers/drink/drink_handlers.c \
    routing/handlers/order/order_handlers.c \
    routing/handlers/payment/payment_handlers.c \
    routing/middleware/middleware.c \
    utils/json_helper/json_helper.c \
    utils/jwt_helper/jwt_helper.c \
    utils/http_helper/http_helper.c \
    utils/file_helper/file_helper.c \
    utils/crypt_helper/crypt_helper.c \
    utils/datetime_helper/datetime_helper.c \
    models/DAOs/user/user_dao.c \
    models/DAOs/drink/drink_dao.c \
    models/DAOs/order/order_dao.c \
    models/DAOs/payment/payment_dao.c \
    -ljson-c \
    -lpq \
    -ljwt \
    -lcrypt
