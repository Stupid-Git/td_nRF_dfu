#ifndef __W_SLIP_H
#define __W_SLIP_H


#include <stdint.h>

#include "../src/myutils.h"

#if defined __cplusplus
extern "C" {
#endif

int32_t   make_HciPacket_cb16(uint8_t* pDin, uint16_t lenDin, cb16_t* cbOut, uint16_t* plenDout );

void      malloc_HciPacket_Set_sequence_number(uint32_t sequence_number);
buf32_t*  malloc_HciPacket(buf32_t* pFrame);

buf32_t*  malloc_frame_DFU_START_PACKET     (int32_t program_mode, int32_t softdevice_size, int32_t bootloader_size, int32_t app_size);
buf32_t*  malloc_frame_DFU_INIT_PACKET      (buf32_t* init_packet);
buf32_t*  malloc_frame_DFU_DATA_PACKET      (uint8_t* buffer, uint32_t length);
buf32_t*  malloc_frame_DFU_STOP_DATA_PACKET (void); //DFU_END_PACKET


#if defined __cplusplus
}
#endif

#endif //__W_SLIP_H
