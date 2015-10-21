#ifndef __UART_XX_H
#define __UART_XX_H


//#include "serial_nrf_transport.h"

#include <stdint.h>


#if defined __cplusplus
extern "C" {
#endif

    
struct Uart_xx_s;
typedef struct Uart_xx_s Uart_xx_t;


typedef struct Uart_xx_s
{
    int         (*open)(Uart_xx_t* pT, void *transport_config);
    int         (*close)(Uart_xx_t* pT);
    int         (*send_data)(Uart_xx_t* pT, uint8_t packet_type, uint8_t *packet, int size);
    void        (*register_handler_DataReceived)(Uart_xx_t* pT, void (*handler)(Uart_xx_t* pT, void* extraParam1));
    const char* (*get_transport_name)(Uart_xx_t* pT);

    Uart_xx_t*  pT;
    void*       config; //transport config
    void*       m_parent;

    //TODO serDat_t* pSD;

    uint8_t   hci_packet_buffer_reserved;
    // host to controller flow control
    uint8_t  num_cmd_packets;

    void (*pkt_handler_0x01)(uint8_t *packet, uint16_t size);
    void (*pkt_handler_T2)(uint8_t *packet, uint16_t size);
    void (*pkt_handler_SLIP)(uint8_t *packet, uint16_t size);    

}Uart_xx_t;


//-----------------------------------------------------------------------------
#include <Windows.h>
struct Uart_NW_s;
typedef struct Uart_NW_s Uart_NW_t;
typedef int32_t (*uart_NW_handler)(Uart_NW_t* pU, uint8_t *packet, uint16_t size);

typedef struct Uart_NW_s
{
    int         (*open)(Uart_xx_t* pT, void *transport_config);
    int         (*close)(Uart_xx_t* pT);
    int         (*send_data)(Uart_xx_t* pT, uint8_t packet_type, uint8_t *packet, int size);
    void        (*register_handler_DataReceived)(Uart_xx_t* pT, void (*handler)(Uart_xx_t* pT, void* extraParam1));
    const char* (*get_transport_name)(Uart_xx_t* pT);

    Uart_xx_t*  pT;
    void*       config; //transport config
    void*       m_parent;

    //TODO serDat_t*   pSD;

    uint8_t     hci_packet_buffer_reserved;
    // host to controller flow control
    uint8_t     num_cmd_packets;

    int32_t     (*pkt_handler_0x01)(Uart_NW_t* pU, uint8_t *packet, uint16_t size);
    int32_t     (*pkt_handler_T2)  (Uart_NW_t* pU, uint8_t *packet, uint16_t size);
    int32_t     (*pkt_handler_SLIP)(Uart_NW_t* pU, uint8_t *packet, uint16_t size);    

    HANDLE      hComm;
    TCHAR       szPort[12];
    uint32_t    baudRate;

}Uart_NW_t;



int32_t uart_NW_Init_A(Uart_NW_t* U, char *portName, uint32_t baudRate);
int32_t uart_NW_Init_N(Uart_NW_t* U, int portNum, uint32_t baudRate);
int32_t uart_NW_Init(Uart_NW_t* U, void *config);
int32_t uart_NW_Open(Uart_NW_t* U);
bool    uart_NW_IsOpen(Uart_NW_t* U);
int32_t uart_NW_Close(Uart_NW_t* U);

int32_t uart_NW_register_pkt_handler_0x01(Uart_NW_t* U, uart_NW_handler handler);
int32_t uart_NW_register_pkt_handler_T2  (Uart_NW_t* U, uart_NW_handler handler);
int32_t uart_NW_register_pkt_handler_SLIP(Uart_NW_t* U, uart_NW_handler handler);

int32_t  uart_NW_Write    (Uart_NW_t* U, uint8_t *packet, uint16_t offset, uint16_t size);
int32_t  uart_NW_Read     (Uart_NW_t* U, uint8_t *packet, uint16_t offset, uint16_t size);

uint32_t uart_NW_Write_A  (Uart_NW_t* U, uint8_t *p_data, uint32_t dwNumberOfBytesToWrite, uint32_t *p_dwBytesWritten);
uint32_t uart_NW_Read_A   (Uart_NW_t* U, uint8_t *p_data, uint32_t dwNumberOfBytesToRead,  uint32_t *p_dwBytesRead);
uint32_t uart_NW_IsCTS_ON (Uart_NW_t* U);

#if defined __cplusplus
}
#endif

#endif //__UART_XX_H
