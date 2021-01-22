#include "read_request.h"

#include <arpa/inet.h>
#include <errno.h>
#include <sys/socket.h>
#include <unistd.h>

#include <string.h>

static char read_buf[1640];
static int save_conn = -1;

int read_request(int conn, char **buf) {
    memset(read_buf, 0, sizeof(read_buf));
    *buf = 0;
    if (conn >= 0) {
        save_conn = conn;
    }
    if (save_conn == -1) {
        return -1;
    }
    int ret = read(save_conn, read_buf, sizeof(read_buf));
    if (ret > 0) {
        *buf = read_buf;
    }
    return ret;
}
