#include "http_server.h"

#include <arpa/inet.h>
#include <errno.h>
#include <sys/socket.h>
#include <unistd.h>


#include "fs.h"
#include "http_read_parser.h"
#include "http_header_parser.h"
#include "read_request.h"

#include <string.h>
#include <stdio.h>
#include <stdbool.h>

const uint16_t server_port = 8080;
const int listen_backlog = 5;

static int create_socket(void);
static int give_new_connection(int listen_fd);
static void read_handler(int fd);
static int sendContent (int conn, const char *out_buf, int left);


void https_initialize(void) {
    int fd = create_socket();

    for (;;) {
        int con_fd = give_new_connection(fd);
        read_handler(con_fd);
        close(con_fd);
    }
}


static int create_socket(void) {
    struct sockaddr_in local;
    int listen_fd;

    memset(&local, 0, sizeof(local));
    local.sin_family = AF_INET;
    local.sin_port = htons(server_port);
    local.sin_addr.s_addr = INADDR_ANY;

    listen_fd = socket(local.sin_family, SOCK_STREAM, 0);

    int err = -1;

    err = bind(listen_fd, (struct sockaddr *)&local, sizeof(local));

    err = listen(listen_fd, listen_backlog);
    (void)err;

    return listen_fd;
}


static int give_new_connection(int listen_fd) {
    struct sockaddr_in from;
    int fromlen;

    int conn_fd = accept(listen_fd, (struct sockaddr *)&from, (socklen_t *)&fromlen);

    if (conn_fd < 0) {
        int err = errno;
        if (err == EINTR || err == EAGAIN) {
            return -1;
        }
    }
    return conn_fd;
}


static char *buf1;
enum {
    OUT_BUF_LEN = 2048,
    IN_BUF_LEN = 4096
};
static char read_buf[IN_BUF_LEN];
static char out_buf[OUT_BUF_LEN];
static uint16_t left_data = 0;

static void read_handler(int conn) {
    memset(read_buf, 0, sizeof(read_buf));
    int ret = read_request(conn, &buf1);
    memcpy(read_buf, buf1, ret);

    if (ret <= 0) {
        return;
    }
    uint16_t total_len = ret;
    bool res = false;
    struct fs_file file = http_read_parser(read_buf, total_len, out_buf, &left_data, &res);

    while(!res) {
        ret = read_request(conn, &buf1);
        if (ret <= 0) {
            return;
        }

        if (IN_BUF_LEN - total_len > ret) {
            memcpy(&read_buf[total_len], buf1, ret);
        } else {
            return;
        }
        total_len += ret;
        file = http_read_parser(read_buf, total_len, out_buf, &left_data, &res);
    }

    sendContent(conn, out_buf, (int32_t)strlen(out_buf));
    if (file.len) {
        sendContent(conn, file.data, file.len - 1);
    }

    return;
}


static int sendContent (int conn, const char *out_buf, int left) {
    int ret = 0, total = 0, len = left;

    while (total < len) {
        ret = send(conn, out_buf + total, (size_t)left, 0);

        if (ret == -1) {
            ret = send(conn, out_buf + total, left, 0);
            if (ret == -1) {
                break;
            }
        }
        if (ret == 0) {
            break;
        }

        total += ret;
        left -= ret;
    }

    return left;
}
