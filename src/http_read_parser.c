#include "http_read_parser.h"
#include "http_parser.h"
#include "http_method_parser.h"
#include "fs.h"
#include <string.h>
#include "http_search_buffer.h"
#include "http_header_parser.h"
#include "http_body_parser.h"
#include "base64.h"

#include <stdio.h>

const uint16_t max_body_len = 3072;
const uint16_t max_uri_len = 512;
static char body[3072] = {0};

struct fs_file httpParser(char *buf1, uint16_t in_data_len, char *out_buf, uint16_t *left_data, bool *ok) {
    struct fs_file file = {NULL, 0, 0, NULL};
    *left_data = 0;
    *ok = true; // запрос полностью обработан, можно посылать ответ
    char *const start_buf = buf1;

    tripSpaceChar(&buf1);
    method_t method = getMethod(&buf1);
    if (method == M_NOT_SUP) {
        error_501(out_buf);
        return file;
    }

    struct buffer uri = getUri(&buf1);
    if (uri.len > max_uri_len) {
        error_414(out_buf);
        return file;
    } else if (uri.len == 0) {
        error_400(out_buf);
        return file;
    }

    ext_t ext = E_NOT_SUP;
    ext = getDataExtensionRequest(uri.data, uri.len);
    if (ext == E_NOT_SUP) {
        error_415(out_buf);
        return file;
    }

    version_t http_version = getHttpVersion(&buf1);
    if (http_version == V_UNKNOWN) {
        error_505(out_buf);
        return file;
    }
    uint16_t chunk_len = 0;
    if (method == M_POST) {
        struct buffer httpd_headers = getHeaders(&buf1);

        uint16_t size_http_preambul = (uint16_t)(buf1 - start_buf);
        uint16_t body_len = in_data_len - size_http_preambul;

        struct headers *headers = headerParser(&httpd_headers);

        if (max_body_len < headers->content_lenght) {
            error_413(out_buf);
            return file;
        }
        /* нужно вычитать всё тело, прежде чем обрабатывать */
        if (headers->content_lenght > body_len) {
            *ok = false;
            *left_data = headers->content_lenght - body_len;
            return file;
        }

        /* все данные в post запросе закодированы base64 */
        chunk_len = decode(buf1, body_len, body, 2048);
    }

    file = methodHandler(ext, uri.data, uri.len, out_buf, body, chunk_len, method);
    return file;
}
