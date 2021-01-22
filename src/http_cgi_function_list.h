#ifndef http_cgi_function_list_h_
#define http_cgi_function_list_h_

#include <stdint.h>


typedef void cgi_func(char *buf, char **param, uint8_t len_param);


#define MAP_CGI_FUNCTION(XX)                                                   \
    XX(GET_VERSION, "/get_version.cgi", get_version, 1)                        \
    XX(CHECK_AUTHORIZATION, "/check_authorizations.cgi", check_authorization, 1)\
    XX(SEND_404, "/send404.cgi", send404, 1)

enum cgi{
#define XX(name, request, function, allow) CGI_##name,
    MAP_CGI_FUNCTION(XX)
#undef XX
        CGI_Q
};


#define XX(name, request, function, allow)                                     \
    void function(char *buf, char **param, uint8_t len_param);
MAP_CGI_FUNCTION(XX)
#undef XX


#endif
