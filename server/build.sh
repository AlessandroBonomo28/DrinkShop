#!/bin/bash

# ljson = libreria json
# lpq = libreria di postgres
# Compila il server
gcc -o server \
    server.c \
    routing/router.c \
    routing/handlers.c \
    utils/json_helper.c \
    -ljson-c \
    -lpq

