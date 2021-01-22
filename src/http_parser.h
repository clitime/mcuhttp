#ifndef HTTP_PARSER_H_
#define HTTP_PARSER_H_

#include "http_search_buffer.h"
#include "http_base.h"


void trip_space_char(char **buf);
method_t get_method(char **m);
version_t get_http_version(char **m);

static inline struct buffer get_uri(char **m) {
    return search_in_buffer(m, where_uri);
}


static inline struct buffer get_headers(char **m) {
    return search_in_buffer(m, where_header);
}

ext_t get_data_extension_request(char *url, uint16_t len);

#endif
