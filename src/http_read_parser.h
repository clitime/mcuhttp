#ifndef http_read_parser_h_
#define http_read_parser_h_

#include <stdbool.h>
#include <stdint.h>

struct fs_file http_read_parser(char *, uint16_t , char *, uint16_t *, bool *);


#endif
