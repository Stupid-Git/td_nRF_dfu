#ifndef __DFU_HCI_PACKETS_H
#define __DFU_HCI_PACKETS_H

#include <stdint.h>

#if defined __cplusplus
extern "C" {
#endif


    
typedef struct serial_medium_s
{
    bool   (*is_open)(void);
    int    (*open)(void *transport_config);
    int    (*close)(void *transport_config);
    int    (*send_packet)(uint8_t packet_type, uint8_t *packet, int size);
    void   (*register_packet_handler)(void (*handler)(uint8_t packet_type, uint8_t *packet, uint16_t size));
    const char * (*get_transport_name)(void);
    // custom extension for UART transport implementations
    int    (*set_baudrate)(uint32_t baudrate);
    // support async transport layers, e.g. IRQ driven without buffers
    int    (*can_send_packet_now)(uint8_t packet_type);


    int32_t  (*_Init_A)      (void* U/*Uart_NW_t* U*/, char* portName, uint32_t baudRate);
    int32_t  (*_Init_N)      (void* U/*Uart_NW_t* U*/, int   portNum,  uint32_t baudRate);
    int32_t  (*_Init)        (void* U/*Uart_NW_t* U*/, void* config);
    int32_t  (*_Open)        (void* U/*Uart_NW_t* U*/);
    bool     (*_IsOpen)      (void* U/*Uart_NW_t* U*/);
    int32_t  (*_Close)       (void* U/*Uart_NW_t* U*/);
    uint32_t (*_Read_A_SLIP) (void* U/*Uart_NW_t* U*/, uint8_t *p_data, uint32_t dwNumberOfBytesToRead,  uint32_t *p_dwBytesRead);
    uint32_t (*_Write_A_SLIP)(void* U/*Uart_NW_t* U*/, uint8_t *p_data, uint32_t dwNumberOfBytesToWrite, uint32_t *p_dwBytesWritten);
    uint32_t (*_Read_A_RAW)  (void* U/*Uart_NW_t* U*/, uint8_t *p_data, uint32_t dwNumberOfBytesToRead,  uint32_t *p_dwBytesRead);
    uint32_t (*_Write_A_RAW) (void* U/*Uart_NW_t* U*/, uint8_t *p_data, uint32_t dwNumberOfBytesToWrite, uint32_t *p_dwBytesWritten);

}  serial_medium_t;


//========== NULL ==========
serial_medium_t* serial_medium_Null_instance();

//========== NULL ==========
serial_medium_t* serial_medium_NW_instance();

//========== NULL ==========
serial_medium_t* serial_medium_UW_instance();


#if defined __cplusplus
}
#endif

#endif //__DFU_HCI_PACKETS_H




