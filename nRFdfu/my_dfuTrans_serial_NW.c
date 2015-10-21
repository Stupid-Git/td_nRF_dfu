
#include "my_dfu_transport.h"


#include <stdio.h>

#define VS_BUILD_karel //karel

#ifdef VS_BUILD_karel //karel
//TODO karel #include <strings.h>
#else
#include <strings.h>
#endif //karel

#include <string.h>

#ifdef VS_BUILD_karel //karel
//TODO #include "MissingStuff.h" //karel
#else //karel
#include <unistd.h>   /* UNIX standard function definitions */
#endif //karel

#include <sys/types.h>


//#include "btstack-config.h"
//#include "debug.h"
//#include "serial_nrf_transport.h"

static DfuTrans_Serial_t* hci_transport_usb;


static void serial_transport_ts1(struct timer *ts)
{
    printf("serial_transport_ts1\n");
    serial_nrf_transport_t* pT;

    pT = (serial_nrf_transport_t*)ts->item.user_data;

    pT->DataReceived(pT, 0);
}

static int serial_transport_ds1(struct data_source *ds)
{
    printf("serial_transport_ds1\n");
    serial_nrf_transport_t* pT;

    pT = (serial_nrf_transport_t*)ds->item.user_data;
    pT->DataReceived(pT, 0);

    run_loop_remove_data_source( &pT->m_dataSource_1 );


    return(0);
}

static int32_t async_callback(serial_nrf_transport_t* pT, uint8_t* buffer, uint16_t count ) //struct serUsb_transfer *transfer)
{
    int32_t nWritten;
    int32_t r;
    uint16_t i;
    cb16_t *cb;
    
    cb = &pT->m_cb_DN;

    nWritten = 0;
    for( i=0 ; i<count; i++)
    {
        r = cb16_push( cb, buffer[i] );
        if( r != 1 )
            break;
        nWritten++;
    }

    if( cb16_count( cb ) > 0)
    {
        //==========
        pT->m_timerSource_1.process = serial_transport_ts1;
        pT->m_timerSource_1.item.user_data = pT;
        //pT->m_timerSource_1.item.next = 0;

        //run_loop_set_timer(&pT->m_timerSource_1, AYSNC_POLLING_INTERVAL_MS);
        run_loop_set_timer(&pT->m_timerSource_1, 1); //0); // NOW !!
        run_loop_add_timer(&pT->m_timerSource_1);

        //==========
        pT->m_dataSource_1.fd = 4;
        pT->m_dataSource_1.item.user_data = pT;
        pT->m_dataSource_1.process = serial_transport_ds1;

        run_loop_add_data_source( &pT->m_dataSource_1 );
        //==========
    }

    return(nWritten);
}


static int nrf_open(serial_nrf_transport_t* pT, void *transport_config)
{
//    int r;

    return 0;
}


static int nrf_close(serial_nrf_transport_t* pT, void *transport_config)
{


    return 0;
}


static int nrf_space_available(serial_nrf_transport_t* pT)
{
    uint16_t bytesAvailable;
    cb16_t* cb;

    cb = &pT->m_cb_UP;

    bytesAvailable = cb16_available( cb );

    return( bytesAvailable );
}

static int nrf_send_data(serial_nrf_transport_t* pT, uint8_t packet_type, uint8_t* buffer, int count)
{
    uint16_t i;
    int32_t  nWritten;
    int32_t  r;
    cb16_t*  cb;

/*
    // TOO SIMPLE - assumes infinite buffer
    cb = &pT->m_cb_UP;
    nWritten = 0;
    for( i=0 ; i<count; i++)
    {
        cb16_push( cb, buffer[i] );
        nWritten++;
    }

    pT->NRF_SER_DataReceived(pT, 0);
    return(nWritten);
*/    

    cb = &pT->m_cb_UP;
    nWritten = 0;
    for( i=0 ; i<count; i++)
    {
        r = cb16_push( cb, buffer[i] );
        if( r != 1 )
            break;
        nWritten++;
    }

    pT->NRF_SER_DataReceived(pT, 0); //Direct call to callback 

    /*REF
    if( cb16_count( cb ) > 0)
    {
        //==========
        pT->m_timerSource_1.process = serial_transport_ts1;
        pT->m_timerSource_1.item.user_data = pT;
        //pT->m_timerSource_1.item.next = 0;

        //run_loop_set_timer(&pT->m_timerSource_1, AYSNC_POLLING_INTERVAL_MS);
        run_loop_set_timer(&pT->m_timerSource_1, 1); //0); // NOW !!
        run_loop_add_timer(&pT->m_timerSource_1);

        //==========
        pT->m_dataSource_1.fd = 4;
        pT->m_dataSource_1.item.user_data = pT;
        pT->m_dataSource_1.process = serial_transport_ds1;

        run_loop_add_data_source( &pT->m_dataSource_1 );
        //==========
    }
    REF*/
    return(nWritten);
}



static void nrf_register_DataReceived_handler(serial_nrf_transport_t* pT, void (*handler)(serial_nrf_transport_t* pT, void* extraParam1)){
    log_info("registering DataReceived_handler");
    pT->DataReceived = handler;
}

static const char * nrf_get_transport_name(serial_nrf_transport_t* pT)
{
    return "NRF";
}


static int nrf_set_baudrate(serial_nrf_transport_t* pT, uint32_t baudrate)
{
    return(0);
}

static void dummy_data_received_handler(serial_nrf_transport_t* pT, void* extraParam1)
{
}


// single instance
static serial_nrf_transport_t * serial_nrf_transport = NULL;

// get nrf singleton
serial_nrf_transport_t * serial_nrf_transport_instance() {
    if (!serial_nrf_transport) {
        serial_nrf_transport = (serial_nrf_transport_t*) malloc( sizeof(serial_nrf_transport_t));
        serial_nrf_transport->open                          = nrf_open;
        serial_nrf_transport->close                         = nrf_close;
        serial_nrf_transport->send_data                     = nrf_send_data;
        serial_nrf_transport->register_DataReceived_handler = nrf_register_DataReceived_handler;
        serial_nrf_transport->get_transport_name            = nrf_get_transport_name;
        serial_nrf_transport->set_baudrate                  = NULL;
        serial_nrf_transport->space_available               = nrf_space_available;

        serial_nrf_transport->DataReceived                  = dummy_data_received_handler;
        serial_nrf_transport->NRF_SER_DataReceived          = dummy_data_received_handler;

        serial_nrf_transport->m_cb_DN.buffer = serial_nrf_transport->m_cb_DN_buffer;
        serial_nrf_transport->m_cb_DN.capacity = sizeof(serial_nrf_transport->m_cb_DN_buffer);
        serial_nrf_transport->m_cb_DN.rdPtr = 0;
        serial_nrf_transport->m_cb_DN.wrPtr = 0;

        serial_nrf_transport->m_cb_UP.buffer = serial_nrf_transport->m_cb_UP_buffer;
        serial_nrf_transport->m_cb_UP.capacity = sizeof(serial_nrf_transport->m_cb_UP_buffer);
        serial_nrf_transport->m_cb_UP.rdPtr = 0;
        serial_nrf_transport->m_cb_UP.wrPtr = 0;

        serial_nrf_transport->m_timerSource_1.process = 0;

    }

    //bogus
   //NG  cli_cb_Init();
    //bogus

    return serial_nrf_transport;
}



///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////



serial_nrf_transport_t* NRF_SER_get_Instance_n(int n)
{
    serial_nrf_transport_t* pT = 0;

    pT = serial_nrf_transport;

    return(pT);
}

void NRF_SER_Open(serial_nrf_transport_t* pT)
{
}

void NRF_SER_Close(serial_nrf_transport_t* pT)
{
}

int32_t NRF_SER_Write(serial_nrf_transport_t* pT, uint8_t *buffer, uint16_t offset, uint16_t count)
{
    int32_t nWritten = 0;

    nWritten = async_callback( pT, buffer + offset, count);

    return(nWritten);
}

int32_t NRF_SER_Read(serial_nrf_transport_t* pT, uint8_t *buffer, uint16_t offset, uint16_t count)
{
    int32_t nRead = 0;
    uint8_t  c;
    uint16_t i;
    uint16_t size;
    cb16_t* cb;

    cb = &pT->m_cb_UP;
    size = cb16_count(cb);
    if( count > size)
        count = size;

    for( i = 0 ; i< count ; i++)
    {
        cb16_pop(cb, &c);
        *(buffer + i) = c;
    }

    nRead = count;
    return(nRead);
}

void NRF_SER_register_DataReceived_handler(serial_nrf_transport_t* pT, void (*handler)(serial_nrf_transport_t* pT, void* extraParam1))
{
    log_info("registering NRF_SER_DataReceived");
    pT->NRF_SER_DataReceived = handler;
}




//cb16_t m_cb16nrf_UP;
//cb16_t m_cb16nrf_DN;

int32_t cli_cb_Init(void) //serial_nrf_transport_instance
{
    serial_nrf_transport_t* pT = NRF_SER_get_Instance_n(0);
    pT->m_cb_UP.rdPtr = 0;
    pT->m_cb_UP.wrPtr = 0;

    pT->m_cb_DN.rdPtr = 0;
    pT->m_cb_DN.wrPtr = 0;
    /*
    m_cb16nrf_UP.rdPtr = 0;
    m_cb16nrf_UP.wrPtr = 0;

    m_cb16nrf_DN.rdPtr = 0;
    m_cb16nrf_DN.wrPtr = 0;
    */
    return(0);
}

int32_t cli_cbUP_count(void)
{
    int32_t r;
    serial_nrf_transport_t* pT = NRF_SER_get_Instance_n(0);
    r = cb16_count( &pT->m_cb_UP );
    //r = cb16_count( &m_cb16nrf_UP );
    return(r);
}
int32_t cli_cbUP_pop( uint8_t* pc)
{
    int32_t r;
    serial_nrf_transport_t* pT = NRF_SER_get_Instance_n(0);
    r = cb16_pop( &pT->m_cb_UP, pc );
    //r = cb16_pop( &m_cb16nrf_UP, pc );
    return(r);
}


int32_t cli_cbDN_available(void)
{
    int32_t r;
    serial_nrf_transport_t* pT = NRF_SER_get_Instance_n(0);
    r = cb16_available( &pT->m_cb_DN );
    //r = cb16_available( &m_cb16nrf_DN );
    return(r);
}
int32_t cli_cbDN_push( uint8_t c)
{
    int32_t r;
    serial_nrf_transport_t* pT = NRF_SER_get_Instance_n(0);
    r = cb16_push( &pT->m_cb_DN, c );
    //r = cb16_push( &m_cb16nrf_DN, c );
    return(r);
}
int32_t cli_cbDN_DoWrite(void)
{
    int32_t r;
    serial_nrf_transport_t* pT = NRF_SER_get_Instance_n(0);
    r = async_callback( pT, 0, 0);
    return(r);
}





static DfuTrans_Serial_t* hci_transport_usb;


// get usb singleton
DfuTrans_Serial_t*  dfuTrans_serial_NW_instance() {
    if (!hci_transport_usb) {
        hci_transport_usb = (DfuTrans_Serial_t*) malloc( sizeof(hci_transport_t));
        hci_transport_usb->open                          = usb_open;
        hci_transport_usb->close                         = usb_close;
        hci_transport_usb->send_packet                   = usb_send_packet;
        hci_transport_usb->register_packet_handler       = usb_register_packet_handler;
        hci_transport_usb->get_transport_name            = usb_get_transport_name;
        hci_transport_usb->set_baudrate                  = NULL;
        hci_transport_usb->can_send_packet_now           = usb_can_send_packet_now;
    }
    return hci_transport_usb;
}


// single instance
static serial_nrf_transport_t * serial_nrf_transport = NULL;

// get nrf singleton
serial_nrf_transport_t * serial_nrf_transport_instance() {
    if (!serial_nrf_transport) {
        serial_nrf_transport = (serial_nrf_transport_t*) malloc( sizeof(serial_nrf_transport_t));
        serial_nrf_transport->open                          = nrf_open;
        serial_nrf_transport->close                         = nrf_close;
        serial_nrf_transport->send_data                     = nrf_send_data;
        serial_nrf_transport->register_DataReceived_handler = nrf_register_DataReceived_handler;
        serial_nrf_transport->get_transport_name            = nrf_get_transport_name;
        serial_nrf_transport->set_baudrate                  = NULL;
        serial_nrf_transport->space_available               = nrf_space_available;

        serial_nrf_transport->DataReceived                  = dummy_data_received_handler;
        serial_nrf_transport->NRF_SER_DataReceived          = dummy_data_received_handler;

        serial_nrf_transport->m_cb_DN.buffer = serial_nrf_transport->m_cb_DN_buffer;
        serial_nrf_transport->m_cb_DN.capacity = sizeof(serial_nrf_transport->m_cb_DN_buffer);
        serial_nrf_transport->m_cb_DN.rdPtr = 0;
        serial_nrf_transport->m_cb_DN.wrPtr = 0;

        serial_nrf_transport->m_cb_UP.buffer = serial_nrf_transport->m_cb_UP_buffer;
        serial_nrf_transport->m_cb_UP.capacity = sizeof(serial_nrf_transport->m_cb_UP_buffer);
        serial_nrf_transport->m_cb_UP.rdPtr = 0;
        serial_nrf_transport->m_cb_UP.wrPtr = 0;

        serial_nrf_transport->m_timerSource_1.process = 0;

    }

    //bogus
   //NG  cli_cb_Init();
    //bogus

    return serial_nrf_transport;
}
