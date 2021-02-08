#ifndef http_read_parser_h_
#define http_read_parser_h_

#include <stdbool.h>
#include <stdint.h>

struct fs_file httpParser(char *buf1, uint16_t in_data_len, char *out_buf, uint16_t *left_data, bool *ok);


#endif
