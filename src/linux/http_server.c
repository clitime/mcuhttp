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


const uint16_t serverPort = 8080;
const int listenBacklog = 5;

static int createSocket(void);
static int giveNewConnection(int listen_fd);
static void readHandler(int fd);
static int sendContent(int conn, const char *out_buf, int left);


void httpInitialize(void) {
    int fd = createSocket();

    for (;;) {
        int con_fd = giveNewConnection(fd);
        readHandler(con_fd);
        close(con_fd);
    }
}


static int createSocket(void) {
    struct sockaddr_in local;
    int listen_fd;

    memset(&local, 0, sizeof(local));
    local.sin_family = AF_INET;
    local.sin_port = htons(serverPort);
    local.sin_addr.s_addr = INADDR_ANY;

    listen_fd = socket(local.sin_family, SOCK_STREAM, 0);

    int err = -1;

    err = bind(listen_fd, (struct sockaddr *) &local, sizeof(local));

    err = listen(listen_fd, listenBacklog);
    (void) err;

    return listen_fd;
}


static int giveNewConnection(int listen_fd) {
    struct sockaddr_in from;
    int fromlen;

    int conn_fd = accept(listen_fd, (struct sockaddr *) &from, (socklen_t *) &fromlen);

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
    BODY_BUF_LEN = 3072,
    IN_BUF_LEN = 4096
};
static char readBuf[IN_BUF_LEN];
static char outBuf[OUT_BUF_LEN];
static char bodyBuf[BODY_BUF_LEN];
static uint16_t left_data = 0;

static void readHandler(int conn) {
    memset(readBuf, 0, sizeof(readBuf));
    int ret = readRequest(conn, &buf1);
    memcpy(readBuf, buf1, ret);

    if (ret <= 0) {
        return;
    }
    uint16_t total_len = ret;
    bool res = false;

    struct httpParser parser = {
        .inBuf = readBuf,
        .inLen = total_len,
        .outBuf = outBuf,
        .bodyBuf = bodyBuf,
        .maxBodyLen = BODY_BUF_LEN
    };
    struct response response = httpParser(&parser, &left_data, &res);

    while (!res) {
        ret = readRequest(conn, &buf1);
        if (ret <= 0) {
            return;
        }

        if (IN_BUF_LEN - total_len > ret) {
            memcpy(&readBuf[total_len], buf1, ret);
        } else {
            return;
        }
        total_len += ret;
        response = httpParser(&parser, &left_data, &res);
    }

    sendContent(conn, outBuf, (int32_t) strlen(outBuf));
    if (response.dlen) {
        sendContent(conn, response.data, response.dlen - 1);
    }

    return;
}


static int sendContent(int conn, const char *out_buf, int left) {
    int ret = 0, total = 0, len = left;

    while (total < len) {
        ret = send(conn, out_buf + total, (size_t) left, 0);

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
