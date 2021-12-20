#ifndef http_read_parser_h_
#define http_read_parser_h_

#include <stdbool.h>
#include <stdint.h>

struct httpParser {
    char *inBuf;
    char *outBuf;
    char *bodyBuf;
    uint16_t inLen;
    const uint16_t maxBodyLen;
};

struct fs_file httpParser(struct httpParser *const parser, uint16_t *left_data, bool *ok);
// struct fs_file httpParser(char *buf1, uint16_t in_data_len, char *out_buf, uint16_t *left_data, bool *ok);


#endif
