#ifndef mcuhttp_h_
#define mcuhttp_h_

#include <stdbool.h>
#include <stdint.h>
#include "http_base.h"
#include "http_method_parser.h"

enum httpParserResult {
    HTTP_ERR,
    HTTP_DONE,
    HTTP_WAIT_BODY,
};

struct httpParser {
    char *inBuf;
    char *outBuf;
    char *bodyBuf;
    uint16_t inLen;
    const uint16_t maxBodyLen;
    getCgiFn getCgi;
};

struct response httpParser(struct httpParser *const parser, uint16_t *left_data, enum httpParserResult *res);

#endif
