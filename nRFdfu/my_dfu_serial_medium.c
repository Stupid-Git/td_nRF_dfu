

#include "my_dfu_serial_medium.h"


/*
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

}  serial_medium_t;
*/

static bool null_is_open(void)
{
    return false;//true;
}
static int null_open(void *transport_config)
{
    return 0;
}


static int null_close(void *transport_config)
{
    return 0;
}

static int null_can_send_packet_now(uint8_t packet_type)
{
    return 0;
}

static int null_send_packet(uint8_t packet_type, uint8_t * packet, int size)
{
    return -1;
}

static void null_register_packet_handler(void (*handler)(uint8_t packet_type, uint8_t *packet, uint16_t size))
{
    //log_info("registering packet handler");
    //packet_handler = handler;
}

static const char * null_get_transport_name(void){
    return "NULL";
}

static void dummy_handler(uint8_t packet_type, uint8_t *packet, uint16_t size)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
static int32_t null_Init_N(void* U/*Uart_NW_t* U*/, int portNum, uint32_t baudRate)
{
    return(0);
}
static int32_t  null_Init_A (void* U/*Uart_NW_t* U*/, char* portName, uint32_t baudRate)
{
    return(0);
}

static     int32_t null_Init   (void* U/*Uart_NW_t* U*/, void* config)
{
    return(0);
}
static     int32_t  null_Open   (void* U/*Uart_NW_t* U*/)
{
    return(0);
}
static     bool    null_IsOpen (void* U/*Uart_NW_t* U*/)
{
    return false;//true;
}
static     int32_t  null_Close  (void* U/*Uart_NW_t* U*/)
{
    return(0);
}



static uint32_t null_Read_A_SLIP(void* U/*Uart_NW_t* U*/, uint8_t *p_data, uint32_t dwNumberOfBytesToRead,  uint32_t *p_dwBytesRead)
{
    *p_dwBytesRead = dwNumberOfBytesToRead;
    return(0);
}

static uint32_t null_Write_A_SLIP(void* U/*Uart_NW_t* U*/, uint8_t *p_data, uint32_t dwNumberOfBytesToWrite, uint32_t *p_dwBytesWritten)
{
    *p_dwBytesWritten = dwNumberOfBytesToWrite;
    return(0);
}

static uint32_t null_Read_A_RAW(void* U/*Uart_NW_t* U*/, uint8_t *p_data, uint32_t dwNumberOfBytesToRead,  uint32_t *p_dwBytesRead)
{
    *p_dwBytesRead = dwNumberOfBytesToRead;
    return(0);
}

static uint32_t null_Write_A_RAW(void* U/*Uart_NW_t* U*/, uint8_t *p_data, uint32_t dwNumberOfBytesToWrite, uint32_t *p_dwBytesWritten)
{
    *p_dwBytesWritten = dwNumberOfBytesToWrite;
    return(0);
}


static serial_medium_t* serial_medium_null;
#include "stdlib.h" //malloc

//========== NULL ==========
// get Null singleton
serial_medium_t* serial_medium_Null_instance(void)
{
    if (!serial_medium_null) {
        serial_medium_null = (serial_medium_t*) malloc( sizeof(serial_medium_t));
        serial_medium_null->is_open                       = null_is_open;
        serial_medium_null->open                          = null_open;
        serial_medium_null->close                         = null_close;
        serial_medium_null->send_packet                   = null_send_packet;
        serial_medium_null->register_packet_handler       = null_register_packet_handler;
        serial_medium_null->get_transport_name            = null_get_transport_name;
        serial_medium_null->set_baudrate                  = NULL;
        serial_medium_null->can_send_packet_now           = null_can_send_packet_now;

        serial_medium_null->_Init                         = null_Init;
        serial_medium_null->_Init_A                       = null_Init_A;
        serial_medium_null->_Init_N                       = null_Init_N;

        serial_medium_null->_IsOpen                       = null_IsOpen;
        serial_medium_null->_Open                         = null_Open;
        serial_medium_null->_Close                        = null_Close;
        serial_medium_null->_Read_A_SLIP                  = null_Read_A_SLIP;
        serial_medium_null->_Write_A_SLIP                 = null_Write_A_SLIP;
        serial_medium_null->_Read_A_RAW                   = null_Read_A_RAW;
        serial_medium_null->_Write_A_RAW                  = null_Write_A_RAW;

    }
    return serial_medium_null;
}




//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
#include "../src/UartSerial/uart_xx.h"

static Uart_NW_t m_theOneAndOnly_Uart_NW;

static Uart_NW_t* __GET_uart_NW(void)
{
    Uart_NW_t* uart = &m_theOneAndOnly_Uart_NW; //TODO
    return( uart );
}

static int32_t NW_Init_N(void* U/*Uart_NW_t* U*/, int portNum, uint32_t baudRate)
{
    uint32_t r;
    Uart_NW_t* uart = 0;
    uart = __GET_uart_NW();

    r = uart_NW_Init_N( uart, portNum, baudRate );
    return(r);
}
static int32_t  NW_Init_A (void* U/*Uart_NW_t* U*/, char* portName, uint32_t baudRate)
{
    uint32_t r;
    Uart_NW_t* uart = 0;
    uart = __GET_uart_NW();

    r = uart_NW_Init_A( uart, portName, baudRate );
    return(r);
}

static     int32_t NW_Init   (void* U/*Uart_NW_t* U*/, void* config)
{
    uint32_t r;
    Uart_NW_t* uart = 0;
    uart = __GET_uart_NW();

    r = uart_NW_Init( uart, config );
    return(r);
}
static  int32_t  NW_Open(void* U/*Uart_NW_t* U*/)
{
    uint32_t r;
    Uart_NW_t* uart = 0;
    uart = __GET_uart_NW();

    r = uart_NW_Open( uart );
    return(r);
}
static  bool  NW_IsOpen (void* U/*Uart_NW_t* U*/)
{
    bool r;
    Uart_NW_t* uart = 0;
    uart = __GET_uart_NW();

    r = uart_NW_IsOpen( uart );
    return(r);
}
static     int32_t  NW_Close  (void* U/*Uart_NW_t* U*/)
{
    uint32_t r;
    Uart_NW_t* uart = 0;
    uart = __GET_uart_NW();

    r = uart_NW_Close( uart );
    return(r);
}


static uint32_t NW_Read_A_SLIP(void* U/*Uart_NW_t* U*/, uint8_t *p_data, uint32_t dwNumberOfBytesToRead,  uint32_t *p_dwBytesRead)
{
    uint32_t r;
    Uart_NW_t* uart = 0;
    uart = __GET_uart_NW();

    r = uart_NW_Read_A( uart, p_data, dwNumberOfBytesToRead, p_dwBytesRead);
    return(r);
}

static uint32_t NW_Write_A_SLIP(void* U/*Uart_NW_t* U*/, uint8_t *p_data, uint32_t dwNumberOfBytesToWrite, uint32_t *p_dwBytesWritten)
{
    uint32_t r;
    Uart_NW_t* uart = 0;
    uart = __GET_uart_NW();

    r = uart_NW_Write_A( uart, p_data, dwNumberOfBytesToWrite, p_dwBytesWritten);
    return(r);
}

static uint32_t NW_Read_A_RAW(void* U/*Uart_NW_t* U*/, uint8_t *p_data, uint32_t dwNumberOfBytesToRead,  uint32_t *p_dwBytesRead)
{
    uint32_t r;
    Uart_NW_t* uart = 0;
    uart = __GET_uart_NW();

    r = uart_NW_Read_A( uart, p_data, dwNumberOfBytesToRead, p_dwBytesRead);
    return(r);
}

static uint32_t NW_Write_A_RAW(void* U/*Uart_NW_t* U*/, uint8_t *p_data, uint32_t dwNumberOfBytesToWrite, uint32_t *p_dwBytesWritten)
{
    uint32_t r;
    Uart_NW_t* uart = 0;
    uart = __GET_uart_NW();

    r = uart_NW_Write_A( uart, p_data, dwNumberOfBytesToWrite, p_dwBytesWritten);
    return(r);
}

static const char * NW_get_transport_name(void){
    return "NW";
}

static serial_medium_t* serial_medium_NW;

//========== NULL ==========
// get Null singleton
serial_medium_t* serial_medium_NW_instance(void)
{
    Uart_NW_t* uart;

    if (!serial_medium_NW)
    {
        uart = __GET_uart_NW();
        uart->hComm = INVALID_HANDLE_VALUE;

        serial_medium_NW = (serial_medium_t*) malloc( sizeof(serial_medium_t));
        serial_medium_NW->is_open                       = null_is_open;
        serial_medium_NW->open                          = null_open;
        serial_medium_NW->close                         = null_close;
        serial_medium_NW->send_packet                   = null_send_packet;
        serial_medium_NW->register_packet_handler       = null_register_packet_handler;
        serial_medium_NW->get_transport_name            = NW_get_transport_name;
        serial_medium_NW->set_baudrate                  = NULL;
        serial_medium_NW->can_send_packet_now           = null_can_send_packet_now;

        serial_medium_NW->_Init                         = NW_Init;
        serial_medium_NW->_Init_A                       = NW_Init_A;
        serial_medium_NW->_Init_N                       = NW_Init_N;  // NW_Init_N

        serial_medium_NW->_IsOpen                       = NW_IsOpen;
        serial_medium_NW->_Open                         = NW_Open;    // NW_Open
        serial_medium_NW->_Close                        = NW_Close;   // NW_Close
        serial_medium_NW->_Read_A_SLIP                  = NW_Read_A_SLIP;
        serial_medium_NW->_Write_A_SLIP                 = NW_Write_A_SLIP;
        serial_medium_NW->_Read_A_RAW                   = NW_Read_A_RAW;
        serial_medium_NW->_Write_A_RAW                  = NW_Write_A_RAW;

    }
    return serial_medium_NW;
}


////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
#include "../src/UartSerial/uart_xx.h"

static Uart_NW_t m_theOneAndOnly_Uart_NWasUW;

static Uart_NW_t* __GET_uart_UW(void)
{
    Uart_NW_t* uart = &m_theOneAndOnly_Uart_NWasUW; //TODO
    return( uart );
}

// same as NW
static int32_t UW_Init_N(void* U/*Uart_NW_t* U*/, int portNum, uint32_t baudRate)
{
    uint32_t r;
    Uart_NW_t* uart = 0;
    uart = __GET_uart_UW();

    r = uart_NW_Init_N( uart, portNum, baudRate );
    return(r);
}
// same as NW
static int32_t  UW_Init_A (void* U/*Uart_NW_t* U*/, char* portName, uint32_t baudRate)
{
    uint32_t r;
    Uart_NW_t* uart = 0;
    uart = __GET_uart_UW();

    r = uart_NW_Init_A( uart, portName, baudRate );
    return(r);
}
// same as NW
static     int32_t UW_Init   (void* U/*Uart_NW_t* U*/, void* config)
{
    uint32_t r;
    Uart_NW_t* uart = 0;
    uart = __GET_uart_UW();

    r = uart_NW_Init( uart, config );
    return(r);
}
// same as NW
static  int32_t  UW_Open(void* U/*Uart_NW_t* U*/)
{
    uint32_t r;
    Uart_NW_t* uart = 0;
    uart = __GET_uart_UW();

    r = uart_NW_Open( uart );
    return(r);
}
// same as NW
static  bool  UW_IsOpen (void* U/*Uart_NW_t* U*/)
{
    bool r;
    Uart_NW_t* uart = 0;
    uart = __GET_uart_UW();

    r = uart_NW_IsOpen( uart );
    return(r);
}
// same as NW
static     int32_t  UW_Close  (void* U/*Uart_NW_t* U*/)
{
    uint32_t r;
    Uart_NW_t* uart = 0;
    uart = __GET_uart_UW();

    r = uart_NW_Close( uart );
    return(r);
}


static uint32_t UW_Read_A_SLIP(void* U/*Uart_NW_t* U*/, uint8_t *p_data, uint32_t dwNumberOfBytesToRead,  uint32_t *p_dwBytesRead)
{
    uint32_t r;
    Uart_NW_t* uart = 0;
    uart = __GET_uart_UW();

    r = uart_NW_Read_A( uart, p_data, dwNumberOfBytesToRead, p_dwBytesRead);
    return(r);
}

static uint32_t UW_Write_A_SLIP(void* U/*Uart_NW_t* U*/, uint8_t *p_data, uint32_t dwNumberOfBytesToWrite, uint32_t *p_dwBytesWritten)
{
    uint32_t r;
    Uart_NW_t* uart = 0;
    uart = __GET_uart_UW();

    // This is a raw write function
    r = uart_NW_Write_A( uart, p_data, dwNumberOfBytesToWrite, p_dwBytesWritten);
    return(r);
}

static uint32_t UW_Read_A_RAW(void* U/*Uart_NW_t* U*/, uint8_t *p_data, uint32_t dwNumberOfBytesToRead,  uint32_t *p_dwBytesRead)
{
    uint32_t r;
    Uart_NW_t* uart = 0;
    uart = __GET_uart_UW();

    r = uart_NW_Read_A( uart, p_data, dwNumberOfBytesToRead, p_dwBytesRead);
    return(r);
}

static uint32_t UW_Write_A_RAW(void* U/*Uart_NW_t* U*/, uint8_t *p_data, uint32_t dwNumberOfBytesToWrite, uint32_t *p_dwBytesWritten)
{
    uint32_t r;
    Uart_NW_t* uart = 0;
    uart = __GET_uart_UW();

    // This is a raw write function
    r = uart_NW_Write_A( uart, p_data, dwNumberOfBytesToWrite, p_dwBytesWritten);
    return(r);
}

static const char * UW_get_transport_name(void){
    return "UW";
}

static serial_medium_t* serial_medium_UW;

//========== NULL ==========
// get Null singleton
serial_medium_t* serial_medium_UW_instance(void)
{
    Uart_NW_t* uart;

    if (!serial_medium_UW)
    {
        uart = __GET_uart_UW();
        uart->hComm = INVALID_HANDLE_VALUE;

        serial_medium_UW = (serial_medium_t*) malloc( sizeof(serial_medium_t));
        serial_medium_UW->is_open                       = null_is_open;
        serial_medium_UW->open                          = null_open;
        serial_medium_UW->close                         = null_close;
        serial_medium_UW->send_packet                   = null_send_packet;
        serial_medium_UW->register_packet_handler       = null_register_packet_handler;
        serial_medium_UW->get_transport_name            = UW_get_transport_name;
        serial_medium_UW->set_baudrate                  = NULL;
        serial_medium_UW->can_send_packet_now           = null_can_send_packet_now;

        serial_medium_UW->_Init                         = UW_Init;
        serial_medium_UW->_Init_A                       = UW_Init_A;
        serial_medium_UW->_Init_N                       = UW_Init_N;

        serial_medium_UW->_IsOpen                       = UW_IsOpen;
        serial_medium_UW->_Open                         = UW_Open;
        serial_medium_UW->_Close                        = UW_Close;
        serial_medium_UW->_Read_A_SLIP                  = UW_Read_A_SLIP;
        serial_medium_UW->_Write_A_SLIP                 = UW_Write_A_SLIP;
        serial_medium_UW->_Read_A_RAW                   = UW_Read_A_RAW;
        serial_medium_UW->_Write_A_RAW                  = UW_Write_A_RAW;

    }
    return serial_medium_UW;
}


////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////


