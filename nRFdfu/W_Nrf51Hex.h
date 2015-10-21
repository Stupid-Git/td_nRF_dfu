#ifndef __W_NRF51HEX_H
#define __W_NRF51HEX_H

#include <stdint.h>

//#include "cintelhex.h"

#if defined __cplusplus
extern "C" {
#endif



//class HexType:
#define  HexType_SOFTDEVICE   1
#define  HexType_BOOTLOADER   2
#define  HexType_SD_BL        3
#define  HexType_APPLICATION  4


struct nRFhex_s;
typedef struct nRFhex_s nRFhex_t;

struct nRFhex_s
{
    uint32_t sd_address_start;
    uint32_t sd_address_end;

    uint32_t bl_address_start;
    uint32_t bl_address_end;

    uint32_t app_address_start;
    uint32_t app_address_end;
};

uint32_t Nrf51Hex_hex_type_get(nRFhex_t *pih);

bool Nrf51Hex_is_softdevice(nRFhex_t *pih);
bool Nrf51Hex_is_bootloader(nRFhex_t *pih);
bool Nrf51Hex_is_application(nRFhex_t *pih);

#if defined __cplusplus
}
#endif

#endif //__W_NRF51HEX_H
