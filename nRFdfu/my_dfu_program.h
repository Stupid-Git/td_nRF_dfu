#ifndef __DFU_PROGRAM_H
#define __DFU_PROGRAM_H

#include <stdint.h>

#include "my_dfu_transport.h";

#include "my_dfu.h";


#if defined __cplusplus
extern "C" {
#endif


int32_t my_dfu_OverSerial_program( char* fileName, uint32_t Hexfile_type, char* ComportName, uint32_t baudRate, uint32_t flowControl );







#if defined __cplusplus
}
#endif

#endif //__DFU_PROGRAM_H

