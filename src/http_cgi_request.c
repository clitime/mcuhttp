#include <stdlib.h>
#include <string.h>

#include "http_cgi_request.h"


static uint8_t parseParameter(char *param, char **query_param, uint8_t qlen);
static inline char *searchParameter(char *uri, char **new_uri);


void cgiHandler(char *uri, char *out_buf, getCgiFn getCgi) {
    char *param = searchParameter(uri, &uri);
    char *query_param[20];
    uint8_t count_param = parseParameter(param, query_param, 20);

    if (getCgi != NULL) {
        getCgi(uri)(out_buf, query_param, count_param);
    }
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

static inline char *searchParameter(char *uri, char **new_uri) {
    if (strchr(uri, '?')) {
        *new_uri = strtok(uri, "?");
        return strtok(NULL, " ");
    }
    return NULL;
}
