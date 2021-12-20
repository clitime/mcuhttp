#include "http_method_parser.h"
#include "http_cgi_request.h"
#include <string.h>
#include <stdio.h>


#define MAP_HEADER(XX)                                                         \
    XX(HTTP_HEADER_200,                                                        \
       "HTTP/1.1 200 OK\r\nServer: mcuhttp\r\nConnection: close\r\n")         \
    XX(HTTP_HEADER_400,                                                        \
       "HTTP/1.1 400 Bad Request\r\nServer: mcuhttp\r\nConnection: close\r\n")\
    XX(HTTP_HEADER_404,                                                        \
       "HTTP/1.1 404 Not Found\r\nServer: mcuhttp\r\nConnection: close\r\n")  \
    XX(HTTP_HEADER_405, "HTTP/1.1"                                             \
       "405 Method Not Allowed\r\nServer: mcuhttp\r\nConnection: close\r\n")  \
    XX(HTTP_HEADER_413, "HTTP/1.1 413 Request Entity Too Large\r\nServer: "    \
                        "mcuhttp\r\nConnection: close\r\n")                   \
    XX(HTTP_HEADER_414, "HTTP/1.1 414 Request-URI Too Long\r\nServer: "        \
                        "mcuhttp\r\nConnection: close\r\n")                   \
    XX(HTTP_HEADER_415, "HTTP/1.1 415 Unsupported Media Type\r\nServer: "      \
                        "mcuhttp\r\nConnection: close\r\n")                   \
    XX(HTTP_HEADER_500, "HTTP/1.1 500 Internal Server Error\r\nServer: "       \
                        "mcuhttp\r\nConnection: close\r\n")                   \
    XX(HTTP_HEADER_501, "HTTP/1.1 501 Not Implemented\r\nServer: "             \
                        "mcuhttp\r\nConnection: close\r\n")                   \
    XX(HTTP_HEADER_505, "HTTP/1.1 505 HTTP Version not supported\r\nServer: "  \
                        "mcuhttp\r\nConnection: close\r\n")                   \
    XX(CONTENT_UTF,                                                            \
       "Content-type: text/txt; charset=UTF-8\r\nContent-Length: ")            \
    XX(CONTENT_JPG, "Content-type: image/jpeg\r\nContent-Length: ")            \
    XX(CONTENT_PNG, "Content-type: image/png\r\nContent-Length: ")             \
    XX(CONTENT_GIF, "Content-type: image/gif\r\nContent-Length: ")             \
    XX(CONTENT_ICO, "Content-type: image/icon\r\nContent-Length: ")            \
    XX(CONTENT_CSS, "Content-type: text/css\r\nContent-Length: ")              \
    XX(CONTENT_HTML, "Content-type: text/html\r\nContent-Length: ")            \
    XX(CONTENT_TXT,                                                            \
       "Content-type: text/txt; charset=utf-8\r\nContent-Length: ")            \
    XX(CONTENT_JS,                                                             \
       "Content-type: application/x-javascript\r\nContent-Encoding: "          \
       "gzip\r\nContent-Length: ")


enum {
#define XX(name, string) name,
    MAP_HEADER(XX)
#undef XX
    CONTENT_Q,
};

static const char *headerArr[] = {
#define XX(name, string) string,
    MAP_HEADER(XX)
#undef XX
    NULL
};

bool isAuthorization = false;

static inline bool is_slash(const char *c) {
    return (strncmp(c, "/", 1) == 0);
}

void error_413(char *out_buf) {
    const char *header = headerArr[HTTP_HEADER_413];
    const char *content = headerArr[CONTENT_HTML];
    out_buf[0] = '\0';
    sprintf(out_buf, "%s%s\r\n\r\n", header, content);
}


void error_414(char *out_buf) {
    const char *header = headerArr[HTTP_HEADER_414];
    const char *content = headerArr[CONTENT_HTML];
    out_buf[0] = '\0';
    sprintf(out_buf, "%s%s\r\n\r\n", header, content);
}


void error_415(char *out_buf) {
    const char *header = headerArr[HTTP_HEADER_415];
    const char *content = headerArr[CONTENT_HTML];
    out_buf[0] = '\0';
    sprintf(out_buf, "%s%s\r\n\r\n", header, content);
}


void error_400(char *out_buf) {
    const char *header = headerArr[HTTP_HEADER_400];
    const char *content = headerArr[CONTENT_HTML];
    out_buf[0] = '\0';
    sprintf(out_buf, "%s%s\r\n\r\n", header, content);
}


void error_501(char *out_buf) {
    const char *header = headerArr[HTTP_HEADER_501];
    const char *content = headerArr[CONTENT_HTML];
    out_buf[0] = '\0';
    sprintf(out_buf, "%s%s\r\n\r\n", header, content);
}


void error_505(char *out_buf) {
    const char *header = headerArr[HTTP_HEADER_505];
    const char *content = headerArr[CONTENT_HTML];
    out_buf[0] = '\0';
    sprintf(out_buf, "%s%s\r\n\r\n", header, content);
}

const uint8_t preambulaArray[][2] = {
    {HTTP_HEADER_200, CONTENT_UTF},
    {HTTP_HEADER_200, CONTENT_HTML},
    {HTTP_HEADER_200, CONTENT_JPG},
    {HTTP_HEADER_200, CONTENT_PNG},
    {HTTP_HEADER_200, CONTENT_GIF},
    {HTTP_HEADER_200, CONTENT_CSS},
    {HTTP_HEADER_200, CONTENT_ICO},
    {HTTP_HEADER_200, CONTENT_JS},
    {HTTP_HEADER_200, CONTENT_TXT},
    {HTTP_HEADER_404, CONTENT_HTML},
    {HTTP_HEADER_501, CONTENT_HTML}
};

struct fs_file methodHandler(ext_t ext, char *uri, uint16_t len, char *out_buf, char *body, uint16_t body_len, method_t method) {
    struct fs_file file = {NULL, 0};
    out_buf[0] = '\0';

    char file_name[255] = {0};
    if ((len == 1) && is_slash(uri)) {
        strncat(file_name, "/index.html\0", 12);
    } else {
        strncat(file_name, uri, len);
    }

    if (!isAuthorization && ext == E_HTML && strncmp(file_name, "/index.html", sizeof("/index.html"))) {
        if (fsOpen(&file, "/login.html\0")) {
            if (fsOpen(&file, "/404.html\0")) {
                fsClose(&file);
            } else {
                ext = E_NOT_FOUND;
            }
        }
    } else if (!isAuthorization && ext == E_TXT) {
        if (fsOpen(&file, "/txt/login.txt\0")) {
            fsClose(&file);

            if (fsOpen(&file, "/txt/404.txt\0")){
                fsClose(&file);
            } else{
                ext = E_NOT_FOUND;
            }
        }
    } else if ((ext != E_CGI) && fsOpen(&file, file_name)) {
        fsClose(&file);

        if (fsOpen(&file, "/404.html\0")) {
            fsClose(&file);
        } else {
            ext = E_NOT_FOUND;
        }
    }

    const char *content;
    const char *header;
    out_buf[0] = '\0';

    if (method > M_POST) {
        header = headerArr[HTTP_HEADER_405];
        content = headerArr[CONTENT_TXT];
    } else {
        header = headerArr[preambulaArray[(uint8_t)ext][0]];
        content = headerArr[preambulaArray[(uint8_t)ext][1]];
        if ((uint8_t)ext == E_CGI) {
            cgiHandler(uri, out_buf, body, body_len, method);
        }
    }

    size_t outLen = strlen(out_buf);
    if (outLen) {
        size_t hl = strlen(header);
        size_t cl = strlen(content);
        char l[10];
        snprintf(l, 9, "%lu\r\n\r\n", outLen);
        size_t ll = strlen(l);
        memmove(&out_buf[hl+cl+ll], out_buf, outLen);
        memcpy(out_buf, header, hl);
        memcpy(&out_buf[hl], content, cl);
        memcpy(&out_buf[hl+cl], l, ll);
    } else {
        sprintf(out_buf, "%s%s%d\r\n\r\n", header, content, file.len ? file.len - 1 : 0);
    }

    return file;
}
