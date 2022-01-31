#include "http_header_parser.h"
#include <string.h>
#include <stdlib.h>

struct headers headerParser(struct buffer *const buf) {
    uint16_t len = buf->len;
    const char *const headers = buf->data;
    struct headers headers_struct = (struct headers){0};

    char c;
    uint16_t ix = 0;

    for (; ix < len; ix++) {
        c = headers[ix];
        if (c == '\r' || c == '\n') {
            ++ix;
            if (c == '\r' && ix < len && headers[ix] == '\n') {
                ++ix;
            }

            if (strncmp(&headers[ix], "Content-Length:", 15) == 0) {
                ix += 15;
                headers_struct.content_lenght = strtol(&headers[ix], 0, 10);
            } else if (strncmp(&headers[ix], "Connection:", 11) == 0) {
// TODO:
            }
        }
    }
    return headers_struct;
}
