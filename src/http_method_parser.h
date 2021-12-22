#ifndef HTTP_METHOD_PARSER_H_
#define HTTP_METHOD_PARSER_H_

#include "http_parser.h"

#include <stdbool.h>
#include "fs.h"
#include "http_base.h"

struct query {
    method_t method;
    ext_t ext;
    char *uri;
    uint16_t uriLen;
    char *body;
    uint16_t bodyLen;
    char *outBuf;
    uint16_t maxOutLen;
};

struct response {
    const char *data;
    uint32_t dlen;
};

struct response methodHandler(struct query *const query);

void error_400(char *out_buf);
void error_413(char *out_buf);
void error_414(char *out_buf);
void error_415(char *out_buf);
void error_501(char *out_buf);
void error_505(char *out_buf);

#endif
