

#include "dfu_hci_packets.h"

//https://developer.nordicsemi.com/nRF51_SDK/nRF51_SDK_v8.x.x/doc/8.0.0/s110/html/a00102.html


#define DFU_START_PACKET    3

int32_t dfu_hci_make_StartPacket()
{
    return(42);
}