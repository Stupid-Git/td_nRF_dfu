#ifndef __W_SERIALFLASH_H
#define __W_SERIALFLASH_H


#include <stdint.h>


#if defined __cplusplus
extern "C" {
#endif

#define SERIAL_FLASH_SECTOR_SIZE    256
#define SERIAL_FLASH_SECTOR_COUNT   0x400 //Limit to 256K for now 0x1000
#define SERIAL_FLASH_START_ADDRESS  0x00000000
#define SERIAL_FLASH_LAST_ADDRESS   (SERIAL_FLASH_START_ADDRESS + (SERIAL_FLASH_SECTOR_SIZE * (SERIAL_FLASH_SECTOR_COUNT - 1)))


struct W_serialFlash_type1_s;
typedef struct W_serialFlash_type1_s W_serialFlash_type1_t;

typedef struct W_serialFlash_type1_s
{
    int xx;
} W_serialFlash_type1_t;


int32_t W_serialFlash_writeSector256( uint16_t sectorNo, uint8_t *p256, uint16_t len_data );
int32_t W_serialFlash_readSector256( uint16_t sectorNo, uint8_t *p256, uint16_t len_max );

#if defined __cplusplus
}
#endif

#endif //__W_SERIALFLASH_H
