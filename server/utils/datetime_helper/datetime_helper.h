#ifndef DATETIME_HELPER_H
#define DATETIME_HELPER_H

#include <stdio.h>
#include <time.h>
#include <stdbool.h>
// Struttura per rappresentare un oggetto datetime
typedef struct {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
} DateTime;

// Calcola la differenza in secondi tra due oggetti datetime
long int datetime_diff(DateTime dt1, DateTime dt2);

bool dt_greaterThen(DateTime dt1, DateTime dt2);

bool dt_greaterEquals(DateTime dt1, DateTime dt2);

bool dt_equals(DateTime dt1, DateTime dt2);

bool dt_lessThen(DateTime dt1, DateTime dt2);

bool dt_lessEquals(DateTime dt1, DateTime dt2);
// Ottiene il datetime corrente
DateTime datetime_now();

// Formatta un oggetto datetime in una stringa
char* datetime_format(DateTime dt);

// Parsa una stringa in un oggetto datetime
DateTime* datetime_parse(const char* str);


DateTime datetime_addMinutes(DateTime dt, int minutes);

#endif
