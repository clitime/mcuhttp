#include "mcuhttp.h"
#include "http_parser.h"
#include "fs.h"
#include "http_search_buffer.h"
#include "http_header_parser.h"
#include "http_body_parser.h"
#include "base64.h"

#include <string.h>

const uint16_t max_uri_len = 512;

struct response httpParser(struct httpParser *const parser, uint16_t *left_data,
                           enum httpParserResult *res) {
    struct response response = {NULL, 0};
    *left_data = 0;
    *res = HTTP_DONE; // запрос полностью обработан, можно посылать ответ
    char *const start_buf = parser->inBuf;

    tripSpaceChar(&parser->inBuf);
    method_t method = getMethod(&parser->inBuf);
    if (method == M_NOT_SUP) {
        error_501(parser->outBuf);
        return response;
    }

    struct buffer uri = getUri(&parser->inBuf);
    if (uri.len > max_uri_len) {
        error_414(parser->outBuf);
        return response;
    } else if (uri.len == 0) {
        error_400(parser->outBuf);
        return response;
    }

    ext_t ext = E_NOT_SUP;
    ext = getDataExtensionRequest(uri.data, uri.len);
    if (ext == E_NOT_SUP) {
        error_415(parser->outBuf);
        return response;
    }

    version_t http_version = getHttpVersion(&parser->inBuf);
    if (http_version == V_UNKNOWN) {
        error_505(parser->outBuf);
        return response;
    }

    struct buffer httpd_headers = getHeaders(&parser->inBuf);
    uint16_t body_len = 0;
    if (method == M_POST) {
        struct headers headers = headerParser(&httpd_headers);

        uint16_t size_http_preambul = (uint16_t)(parser->inBuf - start_buf);
        body_len = parser->inLen - size_http_preambul;


        if (parser->maxBodyLen < headers.content_lenght) {
            error_413(parser->outBuf);
            return response;
        }
        /* нужно вычитать всё тело, прежде чем обрабатывать */
        if (headers.content_lenght > body_len) {
            *res = HTTP_WAIT_BODY;
            *left_data = headers.content_lenght - body_len;
            return response;
        }
    }

    struct request request = {
        .ext = ext, .method = method, .body = {parser->inBuf, body_len},
        .outBuf = parser->outBuf, .uri = uri, .headers = httpd_headers,
        .getCgi = parser->getCgi
    };
    return methodHandler(&request);
}
