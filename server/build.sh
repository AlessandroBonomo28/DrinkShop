#!/bin/bash

# ljson = libreria json
# lpq = libreria di postgres
# ljwt = libreria jwt token
# Compila il server
gcc -o server \
    server.c \
    routing/router/router.c \
    routing/handlers/handlers.c \
    routing/middleware/middleware.c \
    utils/json_helper/json_helper.c \
    utils/jwt_helper/jwt_helper.c \
    -ljson-c \
    -lpq \
    -ljwt
