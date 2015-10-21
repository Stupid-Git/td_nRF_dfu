#ifndef __DFU_TRANSPORT_H
#define __DFU_TRANSPORT_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "../src/myutils.h"

typedef void (*xxx_event_handler_del)(struct Dfu *P, char*);

typedef struct CL_DfuTransport DfuTransport_t;

typedef int32_t (*CL_register_events_callback_del)(DfuTransport_t *P, int event_type, xxx_event_handler_del callback);


typedef int32_t (*i32_F_pTrans_del)(DfuTransport_t *pT);
typedef bool (*CL_is_open_del)(DfuTransport_t *pT);
typedef void (*v_F_pTrans_del)(DfuTransport_t *pT);
typedef int32_t (*CL_send_start_dfu_del)(DfuTransport_t *P, int program_mode, int softdevice_size, int bootloader_size, int app_size);
typedef int32_t (*CL_send_init_packet_del)(DfuTransport_t *P, buf32_t *init_packet);
typedef int32_t (*CL_send_firmware_del)(DfuTransport_t *P, buf32_t *firmware);

typedef struct CL_DfuTransport
{
    /*
    This class as an abstract base class inherited from when implementing transports.

    The class is generic in nature, the underlying implementation may have missing semantic
    than this class describes. But the intent is that the implementer shall follow the semantic as
    best as she can.
    */

    CL_register_events_callback_del register_events_callback;
    
    CL_is_open_del is_open;
    i32_F_pTrans_del close;
    i32_F_pTrans_del open;
    CL_send_start_dfu_del send_start_dfu;
    CL_send_init_packet_del send_init_packet;
    CL_send_firmware_del send_firmware;
    i32_F_pTrans_del send_validate_firmware;
    i32_F_pTrans_del send_activate_firmware;


} DfuTransport_t;


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// dfu_transport_serial.h
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
typedef bool (*b_F_pv_del)(void*);

typedef struct Serial_Serial_t
{
    b_F_pv_del isOpen;

} Serial_Serial_t;

struct SPINFO { char* name; int handle;};


typedef struct DfuTrans_Serial_t
{
    DfuTransport_t  DfuTransport;

    Serial_Serial_t *serial_port;
    struct SPINFO spInfo;

    char *com_port;
    int   baud_rate;
    int   flow_control; // 1=yes, 0=no
    float timeout;

} DfuTrans_Serial_t;

void init_Dfu_Serial(DfuTrans_Serial_t *gp);
DfuTransport_t * DfuTransportSerial(char *com_port, int baud_rate, bool flow_control);


#ifdef __cplusplus
}
#endif

#endif // __DFU_TRANSPORT_H
