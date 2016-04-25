
#include "stdint.h"
#include "stdio.h"
#include "memory.h"

#include "../src/stdbool.h"


extern "C" uint16_t crc16_compute(uint8_t * p_data, uint16_t size, uint16_t * p_crc);

//*****************************************************************************
//*****************************************************************************
//*****************************************************************************

#include "../src/myutils.h"
#include "W_Nrf51Hex.h"
static nRFhex_t gx_NRFHEX0;

#include "W_ihex_LoadToMem.h"

#define GXIMAGE_BUFFER_CAPACITY 0x8000
static uint8_t m_gxImage_buffer[GXIMAGE_BUFFER_CAPACITY];
static W_ihexMemImage_t m_gxImage0;
static W_ihexMemImage_t* m_gxImage;






#define DATA_INTEGRITY_CHECK_PRESENT   1
#define RELIABLE_PACKET    1
#define HCI_PACKET_TYPE   14

#define DFU_INIT_PACKET     1
#define DFU_START_PACKET    3
#define DFU_DATA_PACKET     4
#define DFU_END_PACKET      5 //DFU_STOP_DATA_PACKET

#define DFU_UPDATE_MODE_NONE    0
#define DFU_UPDATE_MODE_SD      1
#define DFU_UPDATE_MODE_BL      2
#define DFU_UPDATE_MODE_APP     4




#include "../src/UartSerial/uart_xx.h"


#if 1
static uint32_t word_align(uint32_t a, uint32_t b);
static uint32_t word_align(uint32_t a, uint32_t b)
{
    return (((a) + (b - 1)) &~(b - 1));
}

static uint8_t m_start_data_buffer[600];
static buf32_t m_start_data;

#define DEFAULT_SERIAL_PORT_TIMEOUT  1.0  //# Timeout time on serial port read
#define ACK_PACKET_TIMEOUT           1.0  //# Timeout time for for ACK packet received before reporting timeout through event system
#define SEND_INIT_PACKET_WAIT_TIME   1.0  //# Time to wait before communicating with bootloader after init packet is sent
#define SEND_START_DFU_WAIT_TIME    10.0  //# Time to wait before communicating with bootloader after start DFU packet is sent
#define DFU_PACKET_MAX_SIZE        512    //# The DFU packet max size

#endif

#include "../src/myutils.h"
#include "W_slip.h"
extern "C" void W_HciPacket_ZERO(void);

int32_t W_Controller_start_packet_generate(nRFhex_t *pih, cb16_t *cb)
{
    buf32_t *packet = 0;
    uint32_t hex_type;
    
    hex_type = Nrf51Hex_hex_type_get(pih);
    /*
    m_start_data = int32_to_bytes(DFU_START_PACKET);
    m_start_data += int32_to_bytes(hex_type);
    // size of SoftDevice
    m_start_data += int32_to_bytes(word_align(hex_file.sd_address_end, 4) - hex_file.sd_address_start) ;
    // size of BootLoader
    m_start_data += int32_to_bytes(word_align(hex_file.bl_address_end, 4) - hex_file.bl_address_start) ;
    // size of Application
    m_start_data += int32_to_bytes(word_align(hex_file.app_address_end, 4) - hex_file.app_address_start) ;
    */
    m_start_data.buffer = m_start_data_buffer;
    buf32_zero(&m_start_data);
    buf32_cpy(&m_start_data, int32_to_bytes(DFU_START_PACKET), 4);
    buf32_cpy(&m_start_data, int32_to_bytes(hex_type), 4);
    // size of SoftDevice
    buf32_cpy(&m_start_data, int32_to_bytes(word_align(pih->sd_address_end, 4) - pih->sd_address_start), 4);
    // size of BootLoader
    buf32_cpy(&m_start_data, int32_to_bytes(word_align(pih->bl_address_end, 4) - pih->bl_address_start), 4);
    // size of Application
    buf32_cpy(&m_start_data, int32_to_bytes(word_align(pih->app_address_end, 4) - pih->app_address_start), 4);


    buf32_print("start data", &m_start_data);
    uint16_t lenDout;
    uint16_t lenAvailable;
    int32_t lenDout32;

    /*karel*/    W_HciPacket_ZERO(); /*karel*/ 
    lenDout32 = make_HciPacket_cb16( m_start_data.buffer, m_start_data.length, 0, &lenDout );
    lenAvailable = cb16_available( cb );
    if( lenAvailable < lenDout )
        return( lenDout );

    lenDout32 = make_HciPacket_cb16( m_start_data.buffer, m_start_data.length, cb, &lenDout );
    return(0);
    //OLD packet = malloc_HciPacket(&m_start_data);
    //OLD buf32_print("hci Packet", packet);
    //OLD return packet;
}

    /*
    init_packet_generate: init_packet
    01 00 00 00 ff ff ff ff ff ff ff ff 01 00 fe ff
    ef be 00 00
    crc=8bc7  crc=4ee9  crc=502b  crc=ca6f  crc=0727  crc=57e7  crc=cd12  crc=1ae1  crc=4c8b  crc=1cb8  crc=754d  crc=7dc2  crc=73ca
    crc=9a04  crc=3803  crc=aaab  crc=af81  crc=c505  crc=8238  crc=973a  crc=c59f  crc=50fc  crc=a6f5  crc=202c
    
    init_packet_generate: packet
//c0 
//  da 4e 01 d7 
        01 00 00 00
        ff ff ff ff 
        ff ff ff ff 
        01 00 fe ff 
        ef be 00 00
//  2c 20
//c0


c0 
  da 4e 01 d7
    01 00 00 00
    00 00 00 00
    00 00 00 00
    01 00 fe ff
    f2 0f 00 00
  62 6d
c0
*/

// C0 
//   DA 4E 01 D7 
//      01 00 00 00 
//      00 00 00 00 
//      00 00 00 00 
//      01 00 FE FF 
//      F2 0F 00 00
//   62 6D
// C0  



// Onward and upward - eg dfu_test_app_hrm.dat contains this
// FF FF FF FF FF FF FF FF 01 00 FE FF 3F 4B (LSB is on tyhe left)
// From: https://github.com/NordicSemiconductor/pc-nrfutil/blob/master/nordicsemi/dfu/dfu_transport_serial.py




int32_t  W_Controller_init_packet_generate(uint16_t  image_crc, cb16_t *cb)
{
    buf32_t *packet = 0;
    uint8_t init_packet_buffer[600];
    buf32_t init_packet;

    uint16_t hardware_version    = 0x0000; //0xFFFF;
    uint16_t hardware_revision   = 0x0000; //0xFFFF;
    uint32_t application_version = 0x00000000; //0xFFFFFFFF;
    uint16_t softdevice_len      = 0x0001;
    uint16_t softdevice_array    = 0xFFFE;

/*REF
    init_packet = int32_to_bytes(DFU_INIT_PACKET);
    init_packet += int16_to_bytes(hardware_version);
    init_packet += int16_to_bytes(hardware_revision);
    init_packet += int32_to_bytes(application_version);
    init_packet += int16_to_bytes(softdevice_len);
    init_packet += int16_to_bytes(softdevice_array);
    init_packet += int16_to_bytes(image_crc);
    init_packet += int16_to_bytes(0x0000);
*/

    init_packet.buffer = init_packet_buffer;

    buf32_zero(&init_packet);
    //memcpy( &init_packet->buffer[0],int32_to_bytes(DFU_INIT_PACKET), 4); init_packet.len += 4;

    buf32_cpy(&init_packet, int32_to_bytes(DFU_INIT_PACKET), 4);
    buf32_cpy(&init_packet, int16_to_bytes(hardware_version), 2);
    buf32_cpy(&init_packet, int16_to_bytes(hardware_revision), 2);
    buf32_cpy(&init_packet, int32_to_bytes(application_version), 4);
    buf32_cpy(&init_packet, int16_to_bytes(softdevice_len), 2);
    buf32_cpy(&init_packet, int16_to_bytes(softdevice_array), 2);
    buf32_cpy(&init_packet, int16_to_bytes(image_crc), 2);
    buf32_cpy(&init_packet, int16_to_bytes(0x0000), 2);

    //////////////////////////////////////////////
    buf32_print("init_packet_generate: init_packet", &init_packet);
    uint16_t lenDout;
    uint16_t lenAvailable;
    int32_t  lenDout32;

    lenDout32 = make_HciPacket_cb16( init_packet.buffer, init_packet.length, 0, &lenDout );
    lenAvailable = cb16_available( cb );
    if( lenAvailable < lenDout )
        return( lenDout );

    lenDout32 = make_HciPacket_cb16( init_packet.buffer, init_packet.length, cb, &lenDout );
    return(0);
    //OLD buf32_print("init_packet_generate: init_packet", &init_packet);
    //OLD packet = malloc_HciPacket(&init_packet);
    //OLD buf32_print("init_packet_generate: packet", packet);
    //OLD return packet;
}

int32_t W_Controller_stop_packet_generate(cb16_t* cb)
{
    buf32_t *packet = 0;
    uint8_t stop_packet_buffer[600];
    buf32_t stop_packet;
    
    stop_packet.buffer = stop_packet_buffer;
    buf32_zero(&stop_packet);
    buf32_cpy(&stop_packet, int32_to_bytes(DFU_END_PACKET), 4); //DFU_STOP_DATA_PACKET

    buf32_print("init_packet_generate: init_packet", &stop_packet);
    uint16_t lenDout;
    uint16_t lenAvailable;
    int32_t  lenDout32;

    lenDout32 = make_HciPacket_cb16( stop_packet.buffer, stop_packet.length, 0, &lenDout );
    lenAvailable = cb16_available( cb );
    if( lenAvailable < lenDout )
        return( lenDout );

    lenDout32 = make_HciPacket_cb16( stop_packet.buffer, stop_packet.length, cb, &lenDout );
    return(0);

    //OLD packet = malloc_HciPacket(&stop_packet);
    // print "\n\nstop packet data:",
    // for i in packet->buffer:
    //       print hex(ord(i)),
    //OLD return packet;
}


int32_t W_Controller_data_packet_generate_at_offset(uint8_t *bin_image, uint32_t offset512, uint32_t len, cb16_t *cb )//int len)
{
    uint32_t cpy_start;
    uint32_t cpy_len;

    uint8_t temp_packet_buffer[600];
    buf32_t temp_packet;

    temp_packet.buffer = temp_packet_buffer;

    cpy_start = offset512 * 512;
    cpy_len = len - cpy_start;
    if( cpy_len > 512)
        cpy_len = 512;
    
    buf32_zero(&temp_packet);
    buf32_cpy(&temp_packet, int32_to_bytes(DFU_DATA_PACKET), 4 );
    buf32_cpy(&temp_packet, &bin_image[offset512], cpy_len );

    uint16_t lenDout;
    uint16_t lenAvailable;
    int32_t lenDout32;

    lenDout32 = make_HciPacket_cb16( temp_packet.buffer, temp_packet.length, 0, &lenDout );
    lenAvailable = cb16_available( cb );
    if( lenAvailable < lenDout )
        return( lenDout );

    lenDout32 = make_HciPacket_cb16( temp_packet.buffer, temp_packet.length, cb, &lenDout );
    return(0);

    //data_packet = malloc_HciPacket(int32_to_bytes(DFU_DATA_PACKET) + bin_image[i:i+512]);
    //OLD data_packet = malloc_HciPacket(&temp_packet);
}



 void tobinstr(nRFhex_t *pih, uint32_t start, uint32_t end)
 {
 }

void hci_dfu_dummy_load_hex_to_binary(void)
{

    nRFhex_t *pih;
    buf32_t *packet = 0;
//    uint16_t crc;
//    uint16_t bin_image_crc;

//    buf32_t *start_packet;
//    buf32_t *init_packet;
//    buf32_t *stop_packet;
    

    pih = &gx_NRFHEX0;

    pih->bl_address_start = 0;
    pih->bl_address_end = 0;
    pih->sd_address_start = 0;
    pih->sd_address_end = 0;


    printf("gx_start = 0x%08x\n", m_gxImage->gx_start);
    printf("gx_stop  = 0x%08x\n", m_gxImage->gx_stop);

    pih->app_address_start = 0x00018000;
    //pih->app_address_end = 0x00018000 + 0x27cc;
    pih->app_address_end = 0x00018000 + (m_gxImage->gx_stop - m_gxImage->gx_start);

    //self.test_sd()
    /*todo
    ih = Nrf51Hex(m_Controller_file_path);
    
    // if verbose flag is set.
    if( gx_Controller_verbose)
        Controller_verbose_info(ih);


    char *bin_image = "";
    */

    if( Nrf51Hex_is_softdevice(pih) )
    {
        printf("Info: is_softdevice\n");
        //bin_image += tobinstr(pih, pih->sd_address_start, pih->sd_address_end - 1); // remove last address as intelhex module includes the end address.
    }
  
    if( Nrf51Hex_is_bootloader(pih) )
    {
        printf("Info: is_bootloader\n");
        //bin_image += tobinstr(pih, pih->bl_address_start, pih->bl_address_end - 1);
    }

    if( Nrf51Hex_is_application(pih))
    {
        printf("Info: is_application\n");
        //bin_image += tobinstr(pih, pih->app_address_start, pih->app_address_end - 1);
    }
    

}

#include "W_serialFlash.h"
int32_t hci_dfu_binaryImage_to_SLIPinW_serialFlash(uint8_t* pBinImage, uint32_t binImageLength)
{
    int        i;
    int        packetCount;
    nRFhex_t*  pih;
    uint16_t   bin_image_crc;
  
    //---------------------------------------------------------------
    pih = &gx_NRFHEX0;

    //---------------------------------------------------------------
    bin_image_crc = 0xFFFF;
    bin_image_crc = crc16_compute(pBinImage, binImageLength, &bin_image_crc);
    printf("\n");
    printf("(gx_stop - gx_start) gx_len = %d (0x%x), crc=%04x \n", binImageLength, binImageLength, bin_image_crc);

    //---------------------------------------------------------------
    packetCount = 0;
    packetCount = binImageLength / 512;
    if( (binImageLength % 512 ) != 0)
        packetCount++;

    packetCount++; // Start Packet
    packetCount++; // Init Packet
    packetCount++; // Stop Packet
    printf("Expected packet count = %d\n", packetCount);
    //---------------------------------------------------------------

    uint8_t buf256[256]; 
    uint8_t W_buffer[256 + 1 + (4+512+2)*2 + 1]; 
    cb16_t CB0;
    cb16_t* cb = &CB0;
    cb->buffer = W_buffer;
    cb->capacity = sizeof(W_buffer);
    cb->rdPtr = 0;
    cb->wrPtr = 0;

    uint16_t sectorNo;

    sectorNo = 0;
    //-----------------------------------------------------
    //----- Add start packet -----
    int32_t status32;
    status32 = W_Controller_start_packet_generate(pih, cb);
    while( cb16_count(cb) >= 256 )
    {
        for( i=0; i<256; i++) cb16_pop( cb, &buf256[i] );
        status32 = W_serialFlash_writeSector256( sectorNo++, buf256, 256 );
    }

    //----- Add Init Packet -----
    status32 = W_Controller_init_packet_generate(bin_image_crc, cb);
    while( cb16_count(cb) >= 256 )
    {
        for( i=0; i<256; i++) cb16_pop( cb, &buf256[i] );
        status32 = W_serialFlash_writeSector256( sectorNo++, buf256, 256 );
    }

    //----- Add Data (hex image data) packets -----
    //data_packets = Controller_data_packets_generate(pBinImage, binImageLength);
    uint32_t offset512;
    for( offset512 = 0 ; offset512 < binImageLength; offset512+= 512 )
    {
        status32 = W_Controller_data_packet_generate_at_offset(pBinImage, offset512, binImageLength, cb );
        while( cb16_count(cb) >= 256 )
        {
            for( i=0; i<256; i++) cb16_pop( cb, &buf256[i] );
            status32 = W_serialFlash_writeSector256( sectorNo++, buf256, 256 );
        }
    }

    //----- Add stop packet -----
    status32 = W_Controller_stop_packet_generate(cb);
    while( cb16_count(cb) >= 256 )
    {
        for( i=0; i<256; i++) cb16_pop( cb, &buf256[i] );
        status32 = W_serialFlash_writeSector256( sectorNo++, buf256, 256 );
    }

    int32_t remainder;
    remainder = cb16_count(cb);
    for( i=0; i<256; i++) buf256[i] = 0xFF;
    for( i=0; i < remainder; i++) cb16_pop( cb, &buf256[i] );
    status32 = W_serialFlash_writeSector256( sectorNo++, buf256, 256 );

    printf("sector count used = %d\n", sectorNo);
    printf("sector count used = %d\n", sectorNo);
    printf("sector count used = %d\n", sectorNo);
    printf("sector count used = %d\n", sectorNo);
    printf("sector count used = %d\n", sectorNo);
    return(0);
}

int32_t hci_dfu_W_serialFlash_extractSlipPackets(void)
{
    
    int32_t  i;
    int32_t  packetCount;

    uint16_t sectorNo;

    uint8_t  b;
    int32_t  state_nC0s;
    int32_t  status32;

    uint8_t  buf256[256]; 
    uint8_t  W_buffer[256 + 1 + (4+512+2)*2 + 1]; 
    cb16_t   CB0;
    cb16_t*  cb;
    
    cb = &CB0;
    cb->buffer = W_buffer;
    cb->capacity = sizeof(W_buffer);
    //cb->rdPtr = 0;
    //cb->wrPtr = 0;
    cb16_clear(cb);

    printf("cb capacity = %d\n", cb->capacity);


    packetCount = 0;
    sectorNo = 0;
    state_nC0s = 0;
    do {
        status32 = W_serialFlash_readSector256( sectorNo++, buf256, 256 );
        for( i=0; i<256; i++)
        {
            b = buf256[i];  if( b==0xC0) { printf("sectorNo = %2d, i = %3d\n",sectorNo , i); }

            if( ! ( (state_nC0s == 2)  && (b != 0xC0) ) )
            {
                cb16_push( cb, b );
            }
            
            if(state_nC0s == 2)
            {
                if(b == 0xC0)
                {
                    state_nC0s = 0;
                }
                else
                {
                    state_nC0s = -1;
                    break;
                }
            }
            //else
            if(b == 0xC0)
            {
                if(state_nC0s == 0)
                {
                    state_nC0s = 1;
                }
                else
                if(state_nC0s == 1)
                {
                    state_nC0s = 2;
                    packetCount++;
                    printf("Packet No %2d, Length = %d\n", packetCount, cb16_count(cb) );
                    cb16_clear( cb );
                }
            }
        }

    } while(state_nC0s != -1);

    return(0);
}

    


int32_t W_getSlipPacketFromFlash( uint32_t flashAddress, uint8_t *slipBuffer, uint32_t *slipLength, uint32_t slipBufferMaxSize )
{
    int32_t   status32;
    uint16_t  sectorNo;
    uint16_t  sectorAddr;
    uint16_t  packetLen;
    uint8_t   buf256[256]; 
    uint8_t   b;

    // start
    sectorNo = flashAddress / 256;
    sectorAddr = flashAddress % 256;
    packetLen = 0;

    do {
        status32 = W_serialFlash_readSector256( sectorNo++, buf256, 256 );
        if( status32 != 0)
            return(status32);

        b = buf256[sectorAddr];
        if( (packetLen == 0) && (b != 0xC0 )) // initial check
            return(0);

        for( ; sectorAddr < 256; sectorAddr++)
        {
            b = buf256[sectorAddr];
            if( packetLen >= slipBufferMaxSize )
                return(-1);

            slipBuffer[packetLen++] = b;
            *slipLength = packetLen;

            if( (packetLen >= 2)  && (b == 0xC0) )
            {
                return(1); // found one!!
            }
        }
        sectorAddr = 0;

    } while(1);

    return(0);
}
 

int32_t hci_dfu_W_serialFlash_extractSlipPackets_2(void)
{
#define SLIP_BUFFER_SIZE 4096
    int32_t   status32;
    uint32_t  flashAddress;
    uint8_t   slipBuffer[SLIP_BUFFER_SIZE];
    uint32_t  slipLength;
    uint32_t  slipBufferMaxSize;

    printf("==========================================\n");
    printf("hci_dfu_W_serialFlash_extractSlipPackets_2\n");

    slipBufferMaxSize = SLIP_BUFFER_SIZE;
    flashAddress = 0;
    do
    {
        status32 = W_getSlipPacketFromFlash( flashAddress, slipBuffer, &slipLength, slipBufferMaxSize );
        if( status32 == 1 )
        {
            printf("flashAddress 0x%04x, Length = %d\n", flashAddress, slipLength );
            flashAddress += slipLength;
        }
        if( status32 == 0 )
        {
            printf("No more packets from flashAddress 0x%04x\n", flashAddress );
        }
        if( status32 < 0 )
        {
            printf("Something screwed up\n");
        }

    } while( status32 == 1);


    return(status32);
}





extern "C" void Controller_upload_firmware(void);
void Controller_upload_firmware(void)
{
    int32_t r;

    m_gxImage = &m_gxImage0;
    m_gxImage->gx_bin = m_gxImage_buffer;
    m_gxImage->gx_binCapacity = GXIMAGE_BUFFER_CAPACITY; //OR  m_gxImage->gx_binCapacity = sizeof(m_gxImage_buffer);    
    r = W_ihex_LoadToMem( m_gxImage, "C:/ble_nrf51/tandd/nrf51/examples/ble_peripheral/ble_app_K2/pca10028/s110/arm4/_build/nrf51422_xxac_s110.hex" );
    if( r != 0)
    {
        return;
    }

    hci_dfu_dummy_load_hex_to_binary();

    //=====================================================
    //-----------------------------------------------------
//    r = hci_dfu_binaryImage_to_SLIPinW_serialFlash(m_gxImage->gx_bin, m_gxImage->gx_len);
    r = hci_dfu_W_serialFlash_extractSlipPackets();
    r = hci_dfu_W_serialFlash_extractSlipPackets_2();

    //-----------------------------------------------------
    //hci_dfu_binary_to_packetList();


    //=====================================================
    //-----------------------------------------------------
    /* comment for testing*/
    //hci_dfu_packetList_over_serial();
    /**/


}

/* START PACKET 5 x 32bit = 20 Bytes
    m_start_data =  int32_to_bytes(DFU_START_PACKET);
    m_start_data += int32_to_bytes(hex_type);
    m_start_data += int32_to_bytes(word_align(hex_file.sd_address_end, 4) - hex_file.sd_address_start) ;     // size of SoftDevice
    m_start_data += int32_to_bytes(word_align(hex_file.bl_address_end, 4) - hex_file.bl_address_start) ;    // size of BootLoader
    m_start_data += int32_to_bytes(word_align(hex_file.app_address_end, 4) - hex_file.app_address_start) ;    // size of Application
*/
#define  SM_C_START_OFST  0x00000000
#define  SM_C_START_SIZE  20

/* INIT PACKET  4+ 2+ 2+  4+ 2+ 2+ 2+ 2 = 20 Bytes
    init_packet =  int32_to_bytes(DFU_INIT_PACKET);
    init_packet += int16_to_bytes(hardware_version);
    init_packet += int16_to_bytes(hardware_revision);
    init_packet += int32_to_bytes(application_version);
    init_packet += int16_to_bytes(softdevice_len);
    init_packet += int16_to_bytes(softdevice_array);
    init_packet += int16_to_bytes(image_crc);
    init_packet += int16_to_bytes(0x0000);
*/
#define  SM_C_INIT_OFST   0x00000014
#define  SM_C_INIT_SIZE   20

#define  SM_C_DATA_OFST   0x00000028
#define  SM_C_DATA_SIZE   512

typedef enum eCodeStep
{
    eCS_MAKE_START_PKT,
    eCS_MAKE_INIT_PKT,
    eCS_MAKE_DATA_PKT,
        
};

#define SM_SENDBUF_SIZE 6666
uint8_t SM_sendBuf[SM_SENDBUF_SIZE];
cb16_t  SM_cb;

uint32_t SM_imageSize;
uint32_t SM_blockNo;

//nRFhex_t*  pih;
//uint16_t   bin_image_crc;

void SM_Init()
{
    SM_cb.buffer = &SM_sendBuf[0];
    SM_cb.capacity = SM_SENDBUF_SIZE;
    SM_cb.rdPtr = 0;
    SM_cb.wrPtr = 0;
    SM_cb.length = 0;
}

int32_t SM_copyFromFlash( uint32_t src_ofst, uint32_t src_size, uint8_t *buffer )
{
    return( (int32_t)src_size );
}


int32_t SM_copyFromFlash_start_pkt()
{
    uint16_t    lenDout;
    uint16_t    lenAvailable;
    int32_t     lenDout32;
    uint32_t    src_ofst;
    cb16_t    * cb;

    cb = &SM_cb;
    cb16_clear( cb );

    src_ofst = cb->capacity - SM_C_START_SIZE;
    SM_copyFromFlash( SM_C_START_OFST,  SM_C_START_SIZE, cb->buffer + src_ofst );

    /*???*/ SM_imageSize = cb->buffer[src_ofst + 42]; //TODO copy 32bit value for the image size

    lenDout32 = make_HciPacket_cb16( cb->buffer + src_ofst, SM_C_START_SIZE, 0, &lenDout );
    lenAvailable = cb16_available( cb );
    if( lenAvailable < lenDout )
        return( lenDout );

    lenDout32 = make_HciPacket_cb16( cb->buffer + src_ofst, SM_C_START_SIZE, cb, &lenDout );
    return(0);
}


int32_t SM_copyFromFlash_init_pkt()
{
    uint16_t    lenDout;
    uint16_t    lenAvailable;
    int32_t     lenDout32;
    uint32_t    src_ofst;
    cb16_t    * cb;

    cb = &SM_cb;
    cb16_clear( cb );

    src_ofst = cb->capacity - SM_C_INIT_SIZE;
    SM_copyFromFlash( SM_C_INIT_OFST,  SM_C_INIT_SIZE, cb->buffer + src_ofst );

    /*???*/ SM_imageSize = cb->buffer[src_ofst + 42]; //TODO copy 32bit value for the image size

    lenDout32 = make_HciPacket_cb16( cb->buffer + src_ofst, SM_C_INIT_SIZE, 0, &lenDout );
    lenAvailable = cb16_available( cb );
    if( lenAvailable < lenDout )
        return( lenDout );

    lenDout32 = make_HciPacket_cb16( cb->buffer + src_ofst, SM_C_INIT_SIZE, cb, &lenDout );
    return(0);
}

int32_t SM_copyFromFlash_data_pkt(uint32_t blockNo)
{
    uint16_t    lenDout;
    uint16_t    lenAvailable;
    int32_t     lenDout32;
    uint32_t    src_ofst;
    cb16_t    * cb;

    cb = &SM_cb;
    cb16_clear( cb );

    src_ofst = cb->capacity - SM_C_DATA_SIZE;
    SM_copyFromFlash( SM_C_DATA_OFST + (SM_C_DATA_SIZE * blockNo),  SM_C_DATA_SIZE, cb->buffer + src_ofst );

    lenDout32 = make_HciPacket_cb16( cb->buffer + src_ofst, SM_C_DATA_SIZE, 0, &lenDout );
    lenAvailable = cb16_available( cb );
    if( lenAvailable < lenDout )
        return( lenDout );

    lenDout32 = make_HciPacket_cb16( cb->buffer + src_ofst, SM_C_DATA_SIZE, cb, &lenDout );
    return(0);
}

int32_t SM_start_TX()
{
    return(42);
}

void SM_OnTwo_0xC0()
{
}

void SM_codeStepProc( eCodeStep step )
{
    int32_t rc;

    switch( step )
    {
    case eCS_MAKE_START_PKT:
        //pih = &gx_NRFHEX0;
        //W_Controller_start_packet_generate(pih, &SM_cb);
        rc = SM_copyFromFlash_start_pkt();
        break;

    case eCS_MAKE_INIT_PKT:
        rc = SM_copyFromFlash_init_pkt();
        break;

    case eCS_MAKE_DATA_PKT:
        rc = SM_copyFromFlash_data_pkt( SM_blockNo );
        break;
    }
}