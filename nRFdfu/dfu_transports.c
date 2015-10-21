
#include "stdint.h"
#include "stdio.h"
#include "../src/stdbool.h"

#include "dfu_transport.h"


//#include "dfu.h"
#include "dfu1.h"

#include "../src/myutils.h"

//typedef void (*register_events_callback_del)( int, void*);

typedef void (*v_F_v_del)(void);

buf32_t *CL_create_image_size_packet__NOT_USED(DfuTransport_t *P, int32_t softdevice_size, int32_t bootloader_size, int32_t app_size)
{
    /*"""
    Creates an image size packet necessary for sending start dfu.

    @param softdevice_size: Size of SoftDevice firmware
    @type softdevice_size: int
    @param bootloader_size: Size of bootloader firmware
    @type softdevice_size: int
    @param app_size: Size of application firmware
    :return: The image size packet
    :rtype: str
    """*/
    static  uint8_t image_size_packet_buffer[600];
    static  buf32_t image_size_packet;

    image_size_packet.buffer = image_size_packet_buffer;
    image_size_packet.length = 0;
    /*
    softdevice_size_packet = int32_to_bytes(softdevice_size);
    bootloader_size_packet = int32_to_bytes(bootloader_size);
    app_size_packet = int32_to_bytes(app_size);
    image_size_packet = softdevice_size_packet + bootloader_size_packet + app_size_packet;
    */
    return &image_size_packet;
}

void CL___init__(DfuTransport_t *P)
{
    //self.callbacks = {}
}

int32_t CL_open(DfuTransport_t *P)
{
    /*    
    Open a port if appropriate for the transport.
    :return:
    */
    return(0); //   pass
}

int32_t CL_close(DfuTransport_t *P)
{
    /*
    Close a port if appropriate for the transport.
    :return:
    */
    return(0); //   pass
}

int32_t CL_is_open(DfuTransport_t *P)
{
    /*
    Returns if transport is open.

    :return bool: True if transport is open, False if not
    */
    return(0); //   pass
}


int32_t CL_send_start_dfu(DfuTransport_t *P, int program_mode, int softdevice_size, int bootloader_size, int app_size)
{
    /*
    Send packet to initiate DFU communication. Returns when packet is sent or timeout occurs.

    This call will block until packet is sent.
    If timeout or errors occurs exception is thrown.

    :param nordicsemi.dfu.model.HexType program_mode: Type of firmware to upgrade
    :param int softdevice_size: Size of softdevice firmware
    :param int bootloader_size: Size of bootloader firmware
    :param int app_size: Size of application firmware
    :return:
    */
    return(0); //   pass
}




//(((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((
int32_t CL_send_init_packet(DfuTransport_t *P, buf32_t *init_packet)
{
    /*
    Send init_packet to device.

    This call will block until init_packet is sent and transfer of packet is complete.
    If timeout or errors occurs exception is thrown.

    :param str init_packet: Init packet as a str.
    :return:
    */
    return(0); //   pass
}

int32_t CL_send_firmware(DfuTransport_t *P, buf32_t *firmware)
{
    /*
    Start sending firmware to device.

    This call will block until transfer of firmware is complete.
    If timeout or errors occurs exception is thrown.

    :param str firmware:
    :return:
    */
    return(0); //   pass
}

int32_t CL_send_validate_firmware(DfuTransport_t *P)
{
    /*
    Send request to device to verify that firmware has been correctly transferred.

    This call will block until validation is sent and validation is complete.
    If timeout or errors occurs exception is thrown.

    :return bool: True if firmware validated successfully.
    */
    return(0); //   pass
}

int32_t CL_send_activate_firmware(DfuTransport_t *P)
{
    /*
    Send command to device to activate new firmware and restart the device.
    The device will start up with the new firmware.

    Raises an nRFException if anything fails.

    :return:
    */
    return(0); //   pass
}

int32_t CL_register_events_callback(DfuTransport_t *P, int event_type, xxx_event_handler_del callback)
{
    /*
    Register a callback.

    :param DfuEvent callback:
    :return: None
    """
    if event_type not in self.callbacks:
    self.callbacks[event_type] = []

    self.callbacks[event_type].append(callback)
    */
    return(0); //   pass
}

int32_t CL_unregister_events_callback(DfuTransport_t *P, xxx_event_handler_del callback)
{
    /*
    Unregister a callback.

    :param callback: # TODO: add documentation for callback
    :return: None
    """
    for event_type in self.callbacks.keys():
    if callback in self.callbacks[event_type]:
    self.callbacks[event_type].remove(callback)
    */
    return(0); //   pass
}

int32_t CL__send_event(DfuTransport_t *P, int event_type, char **kwargs)
{
    /*
    Method for sending events to registered callbacks.

    If callbacks throws exceptions event propagation will stop and this method be part of the track trace.

    :param DfuEvent event_type:
    :param args: Arguments to callback function
    :return:
    """
    if event_type in self.callbacks.keys():
    for callback in self.callbacks[event_type]:
    callback(**kwargs)
    */
    return(0); //   pass
}





//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

//#############################################################################
//#############################################################################
//#############################################################################
//#############################################################################


void other(Dfu_t *P)
{
    P->dfu_transport->is_open(P->dfu_transport);
    P->dfu_transport->close(P->dfu_transport);
    P->dfu_transport->open(P->dfu_transport);
    /*TODO
    P->dfu_transport->send_start_dfu(program_mode, softdevice_size, bootloader_size, application_size);
    P->dfu_transport->send_init_packet(init_packet);
    P->dfu_transport->send_firmware(firmware);
    */
    P->dfu_transport->send_validate_firmware(P->dfu_transport);
    P->dfu_transport->send_activate_firmware(P->dfu_transport);
}


void dfu_setTransport_Serial(Dfu_t *P, DfuTrans_Serial_t *pT)
{
    //P->dfu_transport = &pT->DfuTransport;
    P->dfu_transport = (DfuTransport_t*)pT;

}
/*
bool dfu_is_open(Dfu_t *P)
{
    return( P->dfu_transport->is_open(P->dfu_transport) );
}
*/



DfuTrans_Serial_t gDFUS;
Dfu_t gDFU;

void Fake_init_Dfu_Serial(DfuTrans_Serial_t *pS)
{
}

void test_Dfu_transport(void)
{
    Dfu_t *pD;
    DfuTrans_Serial_t *pS;


    int program_mode;
    int softdevice_size;
    int bootloader_size;
    int application_size;

    uint8_t Init_packet_buffer[600];
    buf32_t Init_packet;
    buf32_t *init_packet;

    uint8_t Firmware_buffer[600];
    buf32_t Firmware;
    buf32_t *firmware;

    Init_packet.buffer = Init_packet_buffer;
    Firmware.buffer = Firmware_buffer;

    pS = &gDFUS;
    //Real init_Dfu_Serial(pS);
    Fake_init_Dfu_Serial(pS);

    pD = &gDFU;
    dfu_setTransport_Serial(pD, pS);

    //gDFU.dfu_transport->is_open(0);

    if( pD->dfu_transport->is_open(pD->dfu_transport) )
        printf("Is open\n");

    pD->dfu_transport->is_open(pD->dfu_transport);
    pD->dfu_transport->close(pD->dfu_transport);
    pD->dfu_transport->open(pD->dfu_transport);
    
    
    program_mode = 0;
    softdevice_size = 0;
    bootloader_size = 0;
    application_size = 0;
    pD->dfu_transport->send_start_dfu(pD->dfu_transport, program_mode, softdevice_size, bootloader_size, application_size);

    init_packet = &Init_packet;
    init_packet->length = 0;
    pD->dfu_transport->send_init_packet(pD->dfu_transport, init_packet);

    firmware = &Firmware;
    firmware->length = 0;
    pD->dfu_transport->send_firmware(pD->dfu_transport, firmware);
    
    pD->dfu_transport->send_validate_firmware(pD->dfu_transport);
    pD->dfu_transport->send_activate_firmware(pD->dfu_transport);
}



