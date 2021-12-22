#ifndef http_read_parser_h_
#define http_read_parser_h_

#include <stdbool.h>
#include <stdint.h>
#include "http_method_parser.h"

struct httpParser {
    char *inBuf;
    char *outBuf;
    char *bodyBuf;
    uint16_t inLen;
    const uint16_t maxBodyLen;
};

struct response httpParser(struct httpParser *const parser, uint16_t *left_data, bool *ok);

#endif
