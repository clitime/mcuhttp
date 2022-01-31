#ifndef http_base_h_
#define http_base_h_

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    M_GET,
    M_POST,
    M_NOT_SUP
} method_t;


typedef enum {
    V_0_9,
    V_1_0,
    V_1_1,
    V_UNKNOWN
} version_t;

typedef enum {
    E_CGI,
    E_HTML,
    E_JPG,
    E_PNG,
    E_GIF,
    E_CSS,
    E_ICO,
    E_JS,
    E_TXT,
    E_NOT_FOUND,
    E_NOT_SUP
} ext_t;

struct buffer {
    char *data;
    uint16_t len;
};

typedef void cgiFn_t(char *uri, char **param, uint8_t plen, char *body, char *out);

typedef void (*cgi_func)(char *out, char **param, uint8_t len_param);
typedef cgi_func (*getCgiFn)(const char *);

#endif
