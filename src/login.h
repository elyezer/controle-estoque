#ifndef LOGIN_H
#define LOGIN_H

// User level
#define ANONYMOUS 0
#define GUEST 1
#define USER 2
#define ADMIN 3
#define SUPERADMIN 4

// Superadmin
#define USERNAME "admin"
#define PASSWORD "master"

extern unsigned char user_level(request_t * request);

#endif