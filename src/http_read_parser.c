#include "http_read_parser.h"
#include "http_parser.h"
#include "fs.h"
#include <string.h>
#include "http_search_buffer.h"
#include "http_header_parser.h"
#include "http_body_parser.h"
#include "base64.h"

#include <stdio.h>

const uint16_t max_uri_len = 512;

struct response httpParser(struct httpParser *const parser, uint16_t *left_data, bool *ok) {
    struct response response = {NULL, 0};
    *left_data = 0;
    *ok = true; // запрос полностью обработан, можно посылать ответ
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
    uint16_t chunk_len = 0;
    if (method == M_POST) {
        struct buffer httpd_headers = getHeaders(&parser->inBuf);

        uint16_t size_http_preambul = (uint16_t)(parser->inBuf - start_buf);
        uint16_t body_len = parser->inLen - size_http_preambul;

        struct headers *headers = headerParser(&httpd_headers);

        if (parser->maxBodyLen < headers->content_lenght) {
            error_413(parser->outBuf);
            return response;
        }
        /* нужно вычитать всё тело, прежде чем обрабатывать */
        if (headers->content_lenght > body_len) {
            *ok = false;
            *left_data = headers->content_lenght - body_len;
            return response;
        }

        /* все данные в post запросе закодированы base64 */
        chunk_len = decode(parser->inBuf, body_len, parser->bodyBuf);
    }
    struct query query = {
        .ext = ext, .method = method, .body = parser->bodyBuf, .bodyLen = chunk_len,
        .outBuf = parser->outBuf, .uri = uri.data, .uriLen = uri.len
    };
    return methodHandler(&query);
}
