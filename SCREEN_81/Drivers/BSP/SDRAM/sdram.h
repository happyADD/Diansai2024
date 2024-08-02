

#ifndef _SDRAM_H
#define _SDRAM_H

#include "./SYSTEM/sys/sys.h"


#define BANK5_SDRAM_ADDR        ((uint32_t)(0XC0000000))            /* SDRAM¿ªÊ¼µØÖ· */


uint8_t sdram_send_cmd(uint8_t bankx, uint8_t cmd, uint8_t refresh, uint16_t regval);
void sdram_init(void);
void fmc_sdram_write_buffer(uint8_t *pbuf, uint32_t addr, uint32_t n);
void fmc_sdram_read_buffer(uint8_t *pbuf, uint32_t addr, uint32_t n);

#endif



































