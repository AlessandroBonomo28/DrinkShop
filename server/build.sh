#!/bin/bash

# ljson = libreria json
# lpq = libreria di postgres
# Compila il server
gcc -o server \
    server.c \
    utils/json_helper.c \
    -ljson-c \
    -lpq

