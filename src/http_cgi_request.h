#ifndef HTTP_CGI_REQUEST_H_
#define HTTP_CGI_REQUEST_H_

#include <stdint.h>
#include "http_base.h"

void cgi_handler(char *uri, char *out_buf, char *body, uint16_t body_len, method_t method);

#endif
