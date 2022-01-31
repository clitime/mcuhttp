#include "http_search_buffer.h"
#include <string.h>


struct buffer searchInBuffer(char **buf, enum what_search what) {
    struct buffer ret_buf = {.data = *buf, .len = 0};

    uint16_t buf_len = strlen(*buf);
    char *msg = *buf;
    for (uint16_t ix = 0; ix != buf_len; ix++) {
        switch(msg[ix]) {
        case ' ':
            if (what == where_method || what == where_uri) {
                ret_buf.len = ix;
                msg[ix] = '\0';
                ++ix;
                *buf = &msg[ix];
                return ret_buf;
            }
            break;
        case '\r':
            if (what == where_version || what == where_header) {
                if (msg[ix + 1] == '\n' && what == where_version) {
                    ret_buf.len = ix;
                    msg[ix] = '\0';
                    ix += 2;
                    *buf = &msg[ix];
                    return ret_buf;
                } else if (msg[ix + 1] == '\n' && what == where_header) {
                    if (msg[ix + 2] == '\r' && msg[ix + 3] == '\n') {
                        ret_buf.len = ix;
                        msg[ix] = '\0';
                        ix += 4;
                        *buf = &msg[ix];
                        return ret_buf;
                    }
                }
            }
            break;
        default:
            break;
        }
    }
    return ret_buf;
}
