#include "http_parser.h"

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

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

/**
 * @brief getExtType определение типа расширения
 * @param[in] e входящее расширение
 * @return тип расширения
 */
static ext_t getExtType(const char *e) {
    ext_t retVal = E_NOT_SUP;

    if (strncmp(e, "cgi", 3) == 0) {
        retVal = E_CGI;
    } else if ((strncmp(e, "html", 4) == 0) || (strncmp(e, "htm", 3) == 0) ||
               (strncmp(e, "shtml", 5) == 0)) {
        retVal = E_HTML;
    } else if ((strncmp(e, "jpg", 3) == 0)) {
        retVal = E_JPG;
    } else if ((strncmp(e, "png", 3) == 0)) {
        retVal = E_PNG;
    } else if ((strncmp(e, "gif", 3) == 0)) {
        retVal = E_GIF;
    } else if ((strncmp(e, "css", 3) == 0)) {
        retVal = E_CSS;
    } else if ((strncmp(e, "ico", 3) == 0)) {
        retVal = E_ICO;
    } else if ((strncmp(e, "js", 2) == 0)) {
        retVal = E_JS;
    } else if ((strncmp(e, "txt", 3) == 0)) {
        retVal = E_TXT;
    }
    return retVal;
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

    if ((len == 1) && url[0] == '/') {
        ext[0] = '\0';
        strcat(ext, "html\0");
    }
    return getExtType(ext);
}
