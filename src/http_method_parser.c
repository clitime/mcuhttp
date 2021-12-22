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

static inline bool isSlash(const char *c) {
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

const uint8_t titleArray[][2] = {
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

struct response fileHandler(struct query * const query) {
    struct fs_file file = {NULL, 0};

    char file_name[255] = {0};

    if (!isAuthorization && query->ext == E_HTML && strncmp(file_name, "/index.html", sizeof("/index.html"))) {
        strncat(file_name, "/login.html", 12);
    } else if (!isAuthorization && query->ext == E_TXT) {
        strncat(file_name, "/txt/login.txt", 15);
    } else {
        if ((query->uriLen == 1) && isSlash(query->uri)) {
            strncat(file_name, "/index.html", 12);
        } else {
            strncat(file_name, query->uri, query->uriLen);
        }
    }

    if (fsOpen(&file, file_name)) {
        if (fsOpen(&file, query->ext == E_TXT ? "/404.html" : "/txt/404.txt")) {
            fsClose(&file);
        } else {
            query->ext = E_NOT_FOUND;
        }
    }

    return (struct response){.data = file.data, .dlen = file.len};
}

struct response methodHandler(struct query * const query) {
    struct response response = {NULL, 0};

    const char *content;
    const char *header;
    if (query->method > M_POST) {
        header = headerArr[HTTP_HEADER_405];
        content = headerArr[CONTENT_TXT];
    } else {
        if (query->ext == E_CGI) {
            query->outBuf[0] = '\0';
            cgiHandler(query->uri, query->outBuf, query->body, query->bodyLen, query->method);

            header = headerArr[titleArray[(uint8_t)query->ext][0]];
            content = headerArr[titleArray[(uint8_t)query->ext][1]];

            size_t outLen = strlen(query->outBuf);
            size_t hl = strlen(header);
            size_t cl = strlen(content);
            char l[10];
            snprintf(l, 9, "%lu\r\n\r\n", outLen);
            size_t ll = strlen(l);
            memmove(&query->outBuf[hl+cl+ll], query->outBuf, outLen);
            memcpy(query->outBuf, header, hl);
            memcpy(&query->outBuf[hl], content, cl);
            memcpy(&query->outBuf[hl+cl], l, ll);
        } else {
            response = fileHandler(query);

            header = headerArr[titleArray[(uint8_t)query->ext][0]];
            content = headerArr[titleArray[(uint8_t)query->ext][1]];
            sprintf(query->outBuf, "%s%s%d\r\n\r\n", header, content, response.dlen ? response.dlen - 1 : 0);
        }
    }

    return response;
}
