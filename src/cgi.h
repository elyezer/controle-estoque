#ifndef CGI_H
#define CGI_H 

typedef struct var_t
{
    char *name;
    char *value;
    struct var_t *next;
} var_t;

typedef enum
{ 
    NOT_SUPPORTED,
    GET,
    POST,
} request_method_t;

typedef struct {
    unsigned int lenght;
    request_method_t method;
    list_t * GET;
    list_t * POST;
    list_t * COOKIES;
} request_t;

typedef struct {
    list_t * headers;
    list_t * body;
    int length;
} response_t;

#define debug(...) fprintf(stderr, __VA_ARGS__)

extern request_t * request_process(int, char const **);
extern void parse_data(char *, list_t **, const char *, const char *);
extern char * unencode(char *);
extern response_t *response_empty();
extern void response_add_header(response_t *, const char *, const char *);
extern void response_write(response_t *, const char *);

#endif
