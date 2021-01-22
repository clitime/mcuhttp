#ifndef http_base_h_
#define http_base_h_

#include <stdbool.h>
extern bool is_authorization;


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

#endif
