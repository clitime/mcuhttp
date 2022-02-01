#include "http_parser.h"

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "http_search_buffer.h"
#include <ctype.h>


void tripSpaceChar(char **buf) {
    uint16_t ix = 0;
    while (isspace(*buf[ix])) {
        ix++;
    }
    *buf = &*buf[ix];
}

/**
 * @brief getMethod анализирует строку на наличие метода
 * @param[in] *m входящее сообщение
 * @return возвращает метод запроса, если запрос не поддерживается, то M_NOT_SUP
 */
method_t getMethod(char **m) {
    struct buffer buffer = searchInBuffer(m, where_method);

    if (strncmp(buffer.data, "GET", sizeof("GET") - 1) == 0) {
        return M_GET;
    } else if (strncmp(buffer.data, "POST", sizeof("POST") - 1) == 0) {
        return M_POST;
    }

    return M_NOT_SUP;
}


version_t getHttpVersion(char **m) {
    struct buffer buffer = searchInBuffer(m, where_version);

    if (strncmp(buffer.data, "HTTP/1.1", 8) == 0) {
        return V_1_1;
    } else if (strncmp(buffer.data, "HTTP/1.0", 8) == 0) {
        return V_1_0;
    } else if (strncmp(buffer.data, "HTTP/0.9", 8) == 0) {
        return V_0_9;
    }
    return V_UNKNOWN;
}


struct extLen {
    const char *ext;
    uint32_t len;
    ext_t ret;
};

const struct extLen extLen[] = {
    {"cgi", 3, E_CGI},
    {"html", 4, E_HTML},
    {"htm", 3, E_HTML},
    {"shtml", 5, E_HTML},
    {"jpg", 3, E_JPG},
    {"png", 3, E_PNG},
    {"gif", 3, E_GIF},
    {"css", 3, E_CSS},
    {"ico", 3, E_ICO},
    {"js", 2, E_JS},
    {"txt", 3, E_TXT}
};
/**
 * @brief getExtType определение типа расширения
 * @param[in] e входящее расширение
 * @return тип расширения
 */
static ext_t getExtType(const char *e) {
    for (size_t i = 0; i < sizeof(extLen)/sizeof(extLen[0]); i++) {
        if (strncmp(e, extLen[i].ext, extLen[i].len) == 0) {
            return extLen[i].ret;
        }
    }
    return E_NOT_SUP;
}



static inline bool isDot(char c) {
    return c == '.';
}

static inline bool isSlash(const char *c) {
    return (strncmp(c, "/", 1) == 0);
}
/**
 * @brief getDataExtensionRequest функция ищет расширение запроса
 * @param[in] *uri  входящая строка сообщения
 * @return возвращает расширение
 *
 * При получении сообщения, функция ищет сообщение запроса, например .html или
 * .js
 */
ext_t getDataExtensionRequest(char *url, uint16_t len) {
    uint32_t index = 0;
    char ext[5];
    memset(ext, 0, sizeof(ext));

    if (strrchr(url, '.')) {
        uint32_t i;

        for (i = 0; i < len; i++) {
            if (isDot(url[i])) {
                index = i + 1;
                break;
            }
        }
        i = index;
        while ((i < len) && ((i - index) < 5)) {
            ext[i - index] = url[i];
            i++;
        }
    }

    if (len == 1 && url[0] == '/') {
        ext[0] = '\0';
        strcat(ext, "html\0");
    }
    return getExtType(ext);
}
