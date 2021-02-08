#ifndef http_header_parser_h_
#define http_header_parser_h_

#include <stdint.h>
#include "http_search_buffer.h"


struct headers {
    uint16_t content_lenght;
};

struct headers * headerParser(struct buffer *const buf);

#endif
