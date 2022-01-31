#ifndef HTTP_METHOD_PARSER_H_
#define HTTP_METHOD_PARSER_H_

#include "http_parser.h"

#include <stdbool.h>
#include "fs.h"
#include "http_base.h"

struct request {
    method_t method;
    ext_t ext;
    struct buffer uri;
    struct buffer body;
    struct buffer headers;
    char *outBuf;
    uint16_t maxOutLen;
    getCgiFn getCgi;
};

struct response {
    const char *data;
    int dlen;
};

struct response methodHandler(struct request *const request);

void error_400(char *out_buf);
void error_413(char *out_buf);
void error_414(char *out_buf);
void error_415(char *out_buf);
void error_501(char *out_buf);
void error_505(char *out_buf);

#endif
