#include "datetime_helper.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Calcola la differenza in secondi tra due oggetti datetime
long int datetime_diff(DateTime dt1, DateTime dt2) {
    setenv("TZ", TIME_ZONE, 1);
    tzset();
    time_t time1, time2;
    struct tm tm1, tm2;

    memset(&tm1, 0, sizeof(struct tm));
    tm1.tm_year = dt1.year - 1900;
    tm1.tm_mon = dt1.month - 1;
    tm1.tm_mday = dt1.day;
    tm1.tm_hour = dt1.hour;
    tm1.tm_min = dt1.minute;
    tm1.tm_sec = dt1.second;
    time1 = mktime(&tm1);

    memset(&tm2, 0, sizeof(struct tm));
    tm2.tm_year = dt2.year - 1900;
    tm2.tm_mon = dt2.month - 1;
    tm2.tm_mday = dt2.day;
    tm2.tm_hour = dt2.hour;
    tm2.tm_min = dt2.minute;
    tm2.tm_sec = dt2.second;
    time2 = mktime(&tm2);

    return (long int)difftime(time2, time1);
}

// Verifica se dt1 è maggiore di dt2
bool dt_greaterThen(DateTime dt1, DateTime dt2) {
    long int diff = datetime_diff(dt2, dt1);
    return diff > 0;
}

// Verifica se dt1 è maggiore o uguale a dt2
bool dt_greaterEquals(DateTime dt1, DateTime dt2) {
    long int diff = datetime_diff(dt2, dt1);
    return diff >= 0;
}

// Verifica se dt1 è uguale a dt2
bool dt_equals(DateTime dt1, DateTime dt2) {
    long int diff = datetime_diff(dt1, dt2);
    return diff == 0;
}

// Verifica se dt1 è minore di dt2
bool dt_lessThen(DateTime dt1, DateTime dt2) {
    long int diff = datetime_diff(dt1, dt2);
    return diff < 0;
}

// Verifica se dt1 è minore o uguale a dt2
bool dt_lessEquals(DateTime dt1, DateTime dt2) {
    long int diff = datetime_diff(dt1, dt2);
    return diff <= 0;
}

// Ottiene il datetime corrente
DateTime datetime_now() {
    setenv("TZ", TIME_ZONE, 1);
    tzset();
    time_t now;
    struct tm* tm_info;
    time(&now);
    tm_info = localtime(&now);

    DateTime dt;
    dt.year = tm_info->tm_year + 1900;
    dt.month = tm_info->tm_mon + 1;
    dt.day = tm_info->tm_mday;
    dt.hour = tm_info->tm_hour;
    dt.minute = tm_info->tm_min;
    dt.second = tm_info->tm_sec;

    // print now
    printf("Current local time and date: %s", asctime(tm_info));
    return dt;
}

// Formatta un oggetto datetime in una stringa
char* datetime_format(DateTime dt) {
    char* str = (char*)malloc(20 * sizeof(char)); // Assuming max length of formatted string is 20
    sprintf(str, "%04d-%02d-%02d %02d:%02d:%02d", dt.year, dt.month, dt.day, dt.hour, dt.minute, dt.second);
    return str;
}

// Parsa una stringa in un oggetto datetime
DateTime* datetime_parse(const char* str) {
    DateTime* dt = (DateTime*)malloc(sizeof(DateTime));
    memset(dt, 0, sizeof(DateTime));

    int result = sscanf(str, "%d-%d-%d %d:%d:%d", &(dt->year), &(dt->month), &(dt->day), &(dt->hour), &(dt->minute), &(dt->second));

    if (result != 6) {
        free(dt);
        return NULL; // Parsing failed
    }

    return dt;
}

// Aggiunge un numero specificato di minuti a un oggetto datetime
DateTime datetime_addMinutes(DateTime dt, int minutes) {
    // imposta timezone
    setenv("TZ", TIME_ZONE, 1);
    tzset();

    time_t timestamp;
    struct tm *timeinfo;
    
    // Converti l'oggetto DateTime in una struttura tm
    struct tm time_struct = {0};
    time_struct.tm_year = dt.year - 1900;
    time_struct.tm_mon = dt.month - 1;
    time_struct.tm_mday = dt.day;
    time_struct.tm_hour = dt.hour;
    time_struct.tm_min = dt.minute;
    time_struct.tm_sec = dt.second;
    
    // Converti la struttura tm in un timestamp
    timestamp = mktime(&time_struct);
    // Aggiungi i minuti specificati al timestamp
    timestamp -= 60*60;
    timestamp += minutes*60;
    
    // Converti il nuovo timestamp in una struttura tm
    timeinfo = localtime(&timestamp);
    
    // Crea un nuovo oggetto DateTime con i valori aggiornati
    DateTime result;
    result.year = timeinfo->tm_year + 1900;
    result.month = timeinfo->tm_mon + 1;
    result.day = timeinfo->tm_mday;
    result.hour = timeinfo->tm_hour;
    result.minute = timeinfo->tm_min;
    result.second = timeinfo->tm_sec;
    
    return result;
}