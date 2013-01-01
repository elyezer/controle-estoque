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

extern unsigned char login_user_level(request_t *);
extern void login_refresh_session(response_t **, unsigned char);

#endif