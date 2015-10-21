#include "W_serialFlash.h"

//static uint8_t m_Flash[0x1000][256];
static uint8_t m_Flash[0x400][256]; // limit to 256K

int32_t W_serialFlash_writeSector256( uint16_t sectorNo, uint8_t *p256, uint16_t len_data )
{
    int i;
    if(sectorNo >= SERIAL_FLASH_SECTOR_COUNT)
        return(-1);
    for(i=0; i<SERIAL_FLASH_SECTOR_SIZE; i++)
    {
        if( i < len_data )
            m_Flash[sectorNo][i] = p256[i];
        else
            m_Flash[sectorNo][i] = 0xFF;
    }
    return(0);
}

int32_t W_serialFlash_readSector256( uint16_t sectorNo, uint8_t *p256, uint16_t len_max )
{
    int i;
    if(sectorNo >= SERIAL_FLASH_SECTOR_COUNT)
        return(-1);
    for(i=0; i<SERIAL_FLASH_SECTOR_SIZE; i++)
    {
        if( i < len_max )
            p256[i] = m_Flash[sectorNo][i];
        else
            ; //can't write out to greater than len_max
    }
    return(0);
}
