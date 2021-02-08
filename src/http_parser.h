#ifndef HTTP_PARSER_H_
#define HTTP_PARSER_H_

#include "http_search_buffer.h"
#include "http_base.h"


void tripSpaceChar(char **buf);
method_t getMethod(char **m);
version_t getHttpVersion(char **m);

static inline struct buffer getUri(char **m) {
    return searchInBuffer(m, where_uri);
}


static inline struct buffer getHeaders(char **m) {
    return searchInBuffer(m, where_header);
}

ext_t getDataExtensionRequest(char *url, uint16_t len);

#endif
