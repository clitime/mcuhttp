#ifndef HTTP_CGI_REQUEST_H_
#define HTTP_CGI_REQUEST_H_

#include <stdint.h>
#include "http_base.h"


void cgiHandler(char *uri, char *out_buf, getCgiFn getCgi);

#endif
