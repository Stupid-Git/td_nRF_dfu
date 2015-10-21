#ifndef __W_IHEXLOADTOMEM_H
#define __W_IHEXLOADTOMEM_H

#include <stdint.h>

#include "cintelhex.h"

#if defined __cplusplus
extern "C" {
#endif

struct W_ihexMemImage_s;
typedef struct W_ihexMemImage_s W_ihexMemImage_t;

typedef struct W_ihexMemImage_s
{

    uint8_t* gx_bin; //[0x4000];
    uint32_t gx_binCapacity;
    uint32_t _gx_size;
    uint32_t gx_start;
    uint32_t gx_stop;
    uint32_t gx_len;
    uint16_t gx_crc;
            
} W_ihexMemImage_t;

int32_t W_ihexLoadToMem( W_ihexMemImage_t *pI, char* filename );

#if defined __cplusplus
}
#endif

#endif //__W_IHEXLOADTOMEM_H
