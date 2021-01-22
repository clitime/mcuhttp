#ifndef base64_h_
#define base64_h_

#include <stdint.h>


int8_t encode(const char *const in, uint16_t in_len, char *const out, uint16_t out_len);
uint16_t decode(const char *const in, uint16_t in_len, char *const out, uint16_t out_len);

#endif
