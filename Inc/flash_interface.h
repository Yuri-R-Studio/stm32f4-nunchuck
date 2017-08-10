/*
 * flash_interface.h
 *
 *  Created on: 8Aug.,2017
 *      Author: yuri
 */

#ifndef FLASH_INTERFACE_H_
#define FLASH_INTERFACE_H_
#include "error_code.h"
// SECTOR 3 because is small and fast to erase the whole sector
#define FLASH_DATA_SECTOR   FLASH_SECTOR_3

#define FLASH_DATA_ADDR_START 0x0800C000UL
#define FLASH_DATA_ADDR_END   0x0800FFFFUL
#define FLASH_DATA_SIZE       (FLASH_DATA_ADDR_END - FLASH_DATA_ADDR_START)
#define USER_FLASH_END_ADDRESS 0x080FFFFF


error_code_t write_flash (uint8_t *src, const uint32_t size);
uint16_t read_flash(uint8_t *src, uint8_t* dst, const uint32_t size);

#endif /* FLASH_INTERFACE_H_ */
