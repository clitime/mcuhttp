#include "http_body_parser.h"


uint16_t getBodySize(char *m) {
    if (m == 0)
        return 0;
    uint16_t c = 0;
    while(m[c]) {
        c++;
    }
    return c;
}
