#ifndef HTTP_METHOD_PARSER_H_
#define HTTP_METHOD_PARSER_H_

#include "http_parser.h"

#include <stdbool.h>
#include "fs.h"
#include "http_base.h"


struct fs_file method_handler(ext_t ext, char *uri, uint16_t len, char *out_buf,
                                char *body, uint16_t body_len, method_t method);

void error_400(char *out_buf);
void error_413(char *out_buf);
void error_414(char *out_buf);
void error_415(char *out_buf);
void error_501(char *out_buf);
void error_505(char *out_buf);

#endif
