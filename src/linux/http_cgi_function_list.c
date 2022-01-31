#include "http_cgi_function_list.h"
#include "version.h"
#include <stdio.h>
#include <string.h>

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


static const char *http_cgi_request[] = {
#define XX(name, request, function, allow) request,
    MAP_CGI_FUNCTION(XX)
#undef XX
        NULL
};


static cgi_func cgi_requests[] = {
#define XX(name, request, function, allow) function,
    MAP_CGI_FUNCTION(XX)
#undef XX
};


static const uint8_t cgi_allow_request[] = {
#define XX(name, request, function, allow) allow,
    MAP_CGI_FUNCTION(XX)
#undef XX
};

cgi_func getCgiFunction(const char *cgi_request) {
    for (uint8_t ix = 0; ix != CGI_Q; ++ix) {
        const char *requst = http_cgi_request[ix];

        if (strncmp(cgi_request, requst, strlen(requst)) == 0) {
            return cgi_requests[ix];
        }
    }
    return cgi_requests[CGI_SEND_404];
}

__attribute__((weak)) void get_version(char *buf, char **param, uint8_t len_param) {
    (void)len_param;
    (void)param;
    sprintf(buf, "%s\n", get_device_version_full());
    return;
}

__attribute__((weak)) void send404(char *buf, char **param, uint8_t len_param) {
    (void)len_param;
    (void)param;
    sprintf(buf, "ok");
    return;
}

__attribute__((weak)) void check_authorization(char *buf, char **param, uint8_t len_param) {
    (void)len_param;
    (void)param;
    sprintf(buf, "ok");
    return;
}
