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

/* TODO: указать размер выходного буффера */
static char tmp_buf[1500] = {0};

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

struct fs_file methodHandler(ext_t ext, char *uri, uint16_t len, char *out_buf, char *body, uint16_t body_len, method_t method) {
    struct fs_file file = {NULL, 0, 0, NULL};
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
    tmp_buf[0] = '\0';

    if (method > M_POST) {
        header = headerArr[HTTP_HEADER_405];
        content = headerArr[CONTENT_TXT];
    } else {
        switch ((uint8_t)ext) {
        case E_HTML:
            header = headerArr[HTTP_HEADER_200];
            content = headerArr[CONTENT_HTML];
            break;
        case E_CGI:
            header = headerArr[HTTP_HEADER_200];
            content = headerArr[CONTENT_UTF];
            cgiHandler(uri, tmp_buf, body, body_len, method);
            break;
        case E_TXT:
            header = headerArr[HTTP_HEADER_200];
            content = headerArr[CONTENT_TXT];
            break;
        case E_JS:
            header = headerArr[HTTP_HEADER_200];
            content = headerArr[CONTENT_JS];
            break;
        case E_CSS:
            header = headerArr[HTTP_HEADER_200];
            content = headerArr[CONTENT_CSS];
            break;
        case E_ICO:
            header = headerArr[HTTP_HEADER_200];
            content = headerArr[CONTENT_ICO];
            break;
        case E_GIF:
            header = headerArr[HTTP_HEADER_200];
            content = headerArr[CONTENT_GIF];
            break;
        case E_JPG:
            header = headerArr[HTTP_HEADER_200];
            content = headerArr[CONTENT_JPG];
            break;
        case E_PNG:
            header = headerArr[HTTP_HEADER_200];
            content = headerArr[CONTENT_PNG];
            break;
        case E_NOT_FOUND:
            header = headerArr[HTTP_HEADER_404];
            content = headerArr[CONTENT_HTML];
            break;
        case E_NOT_SUP:
        default:
            header = headerArr[HTTP_HEADER_501];
            content = headerArr[CONTENT_HTML];
            break;
        }
    }

    if (strlen(tmp_buf)) {
        sprintf(out_buf, "%s%s%u\r\n\r\n", header, content, (uint16_t)strlen(tmp_buf));
        strcat(out_buf, tmp_buf);
    } else {
        sprintf(out_buf, "%s%s%d\r\n\r\n", header, content, file.len ? file.len - 1 : 0);
    }

    return file;
}
