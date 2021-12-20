#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "http_cgi_request.h"
#include "http_cgi_function_list.h"


static const char *http_cgi_request[] = {
#define XX(name, request, function, allow) request,
    MAP_CGI_FUNCTION(XX)
#undef XX
        NULL
};


static cgi_func *cgi_requests[] = {
#define XX(name, request, function, allow) function,
    MAP_CGI_FUNCTION(XX)
#undef XX
};


static const uint8_t cgi_allow_request[] = {
#define XX(name, request, function, allow) allow,
    MAP_CGI_FUNCTION(XX)
#undef XX
};


// static char *query_param[20]; //буфер параметров


static uint8_t parseParameter(char *param, char **query_param, uint8_t qlen);
static enum cgi getCgiFunction(const char *cgi_request);
static inline char *searchParameter(char *uri, char **new_uri);


void cgiHandler(char *uri, char *out_buf, char *body, uint16_t body_len, method_t method) {
    char *param = searchParameter(uri, &uri);

    char *query_param[20];
    uint8_t count_param = parseParameter(param, query_param, 20);

    enum cgi cgi_n = getCgiFunction(uri);
    if (method == M_POST && body) {
        count_param += parseParameter(body, query_param, 20);
    }
    cgi_requests[cgi_n](out_buf, query_param, count_param);
}


static uint8_t parseParameter(char *param, char **query_param, uint8_t qlen) {
    if (param == NULL) {
        return 0;
    }

    uint8_t num = 0;
    char *tmp = NULL;

    if (strchr(param, '&')) {
        tmp = strtok(param, "&");

        while (tmp != NULL && num != qlen) {
            query_param[num] = tmp;
            num++;
            tmp = strtok(NULL, "&");
        }
    } else {
        query_param[num] = param;
        num++;
    }

    return num;
}


static enum cgi getCgiFunction(const char *cgi_request) {
    for (uint8_t ix = 0; ix != CGI_Q; ++ix) {
        const char *requst = http_cgi_request[ix];

        if (strncmp(cgi_request, requst, strlen(requst)) == 0) {
            if (isAuthorization || cgi_allow_request[ix])
                return ix;
            else
                return CGI_CHECK_AUTHORIZATION;
        }
    }
    return CGI_SEND_404;
}


static inline char *searchParameter(char *uri, char **new_uri) {
    if (strchr(uri, '?')) {
        *new_uri = strtok(uri, "?");
        char *p = strtok(NULL, "?");
        return strtok(p, " ");
    }
    return NULL;
}
