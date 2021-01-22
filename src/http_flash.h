#ifndef HTTP_FLASH_H_
#define HTTP_FLASH_H_

#include <stdint.h>

void flash_erase_block(void);
uint8_t flash_write_data(volatile uint32_t *addr, uint32_t *data, uint32_t len);

#endif
