#ifndef USER_DAO_H
#define USER_DAO_H

User* authenticateUser(PGconn* connection, const char* email, const char* password);
User* registerUser(PGconn* connection, const char* email, const char* password);
User* getUserById(PGconn* connection, int id);
User* getUserByEmail(PGconn* connection, const char* email);

#endif