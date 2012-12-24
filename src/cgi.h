#ifndef CGI_H
#define CGI_H 

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE !FALSE
#endif

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
    unsigned int content_length;
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

extern void parse_data(char *, list_t **, const char *, const char *);
extern char * url_decode(char *);
extern request_t * request_empty();
extern void request_process(request_t **);
extern response_t * response_empty(const char *);
extern void response_add_header(response_t **, const char *, const char *);
extern void response_set_cookie(response_t **, const char *, const char *, const char *);
extern void response_write(response_t **, const char *);
extern void response_send(response_t *);
extern void page_include_header(response_t **);
extern void page_include_footer(response_t **);

#endif
