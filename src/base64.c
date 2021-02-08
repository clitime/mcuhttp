#include "base64.h"

/* RFC4648 Table 2. The URL and filename safe base 64 alphabet */
static const char encodeTable[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_=";


static void encodeBlock(const char *const in, char *const out, uint16_t len);
static uint8_t decodeBlock(const char *const in, char *const out, uint16_t len);
/**
 * -1 - длина выходного буфера не кратна 4
 * -2 - длины выходного буфера не достаточно
 * 0 - кодирование произведенно успешно
 */
int8_t encode(const char *const in, uint16_t in_len, char *const out, uint16_t out_len) {
    if (out_len % 4) {
        return -1;
    }

    if ((in_len / 3 == out_len / 4) && (in_len % 3 || (in_len / 3 > out_len / 4))) {
        return -2;
    }

    uint16_t ix_in = 0, ix_out = 0;
    uint16_t len = 3;
    uint16_t len_diff = 0;
    while (ix_in < in_len) {
        len_diff = in_len - ix_in;
        if (len_diff < 3)
            len = len_diff;

        encodeBlock(&in[ix_in], &out[ix_out], len);
        ix_in += 3;
        ix_out += 4;
    }
    return 0;
}


/**
 * |8 7 6 5 4 3 2 1|8 7 6 5 4 3 2 1|8 7 6 5 4 3 2 1| - вход
 * |6 5 4 3 2 1|6 5 4 3 2 1|6 5 4 3 2 1|6 5 4 3 2 1| - выход
 * выходной буфер должен быть кратен 4 байт
 */
static void encodeBlock(const char *const in, char *const out, uint16_t len) {
    if (!len) {
        return;
    }

    uint8_t ix = (uint8_t)(in[0] >> 2);
    out[0] = encodeTable[ix];

    ix = len > 1 ? (uint8_t)((in[0] & 0x03) << 4) | (uint8_t)(in[1] >> 4)
                 : (uint8_t)((in[0] & 0x03) << 4);
    out[1] = encodeTable[ix];

    if (len > 1) {
        ix = len > 2 ? (uint8_t)((in[1] & 0x0f) << 2) | (uint8_t)(in[2] >> 6)
                     : (uint8_t)((in[1] & 0x0f) << 2);
    } else {
        ix = 64;
    }
    out[2] = encodeTable[ix];

    ix = len == 3 ? (uint8_t)(in[2] & 0x3f) : 64;
    out[3] = encodeTable[ix];
}


#include <stdio.h>
/**
 * |6 5 4 3 2 1|6 5 4 3 2 1|6 5 4 3 2 1|6 5 4 3 2 1| - вход
 * |8 7 6 5 4 3 2 1|8 7 6 5 4 3 2 1|8 7 6 5 4 3 2 1| - выход
 * SA== равно 1 символу H (0x48)
 */
static uint8_t decodeBlock(const char *const in, char *const out, uint16_t len) {
    if (!len) {
        return 0;
    }

    char buffer[4] = {0};

    uint8_t buf_len = 0;
    for (uint8_t ix = 0; ix != len; ix++) {
        if (in[ix] != '=') {
            ++buf_len;
        }
        if (in[ix] >= 'A' && in[ix] <= 'Z') {
            buffer[ix] = in[ix] - 'A';
        } else if (in[ix] >= 'a' && in[ix] <= 'z') {
            buffer[ix] = in[ix] - 'a' + 26;
        } else if (in[ix] >= '0' && in[ix] <= '9') {
            buffer[ix] = in[ix] - '0' + 52;
        } else if (in[ix] == '-') {
            buffer[ix] = 62;
        } else if (in[ix] == '_') {
            buffer[ix] = 63;
        } else {
            buffer[ix] = 0;
        }
    }

    out[0] = (uint8_t)(buffer[0] << 2) | (uint8_t)(buffer[1] >> 4);
    out[1] = (uint8_t)(buffer[1] << 4) | (uint8_t)(buffer[2] >> 2);
    out[2] = (uint8_t)(buffer[2] << 6) | buffer[3];
    return buf_len - 1;
}



uint16_t decode(const char *const in, uint16_t in_len, char *const out, uint16_t out_len) {
    if (!in_len) {
        return 0;
    }

    uint16_t ix_in = 0, ix_out = 0;
    uint16_t len = 4;
    uint16_t len_diff = 0;
    while (ix_in < in_len) {
        len_diff = in_len - ix_in;
        if (len_diff < 4)
            len = len_diff;

        ix_out += decodeBlock(&in[ix_in], &out[ix_out], len);
        ix_in += 4;
    }
    return ix_out;
}
