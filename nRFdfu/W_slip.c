#include "W_slip.h"



//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
#include <memory.h>
#include <stdio.h>


//*****************************************************************************
//*****************************************************************************
//*****************************************************************************

static uint8_t* bytePtr_parts_to_four_bytes(uint32_t seq, uint32_t dicp, uint32_t rel, uint32_t pkt_type, uint32_t pkt_len)
{
    static uint8_t ints[4];

    ints[0] = 0;  ints[1] = 0;  ints[2] = 0;  ints[3] = 0;

    ints[0] = (seq) | (((seq+1)%8) << 3) | (dicp << 6) | (rel << 7);
    ints[1] = pkt_type | ((pkt_len & 0x000F) << 4);
    ints[2] = (pkt_len & 0x0FF0) >> 4;
    ints[3] = ( ~(ints[0]+ints[1]+ints[2]+ints[3]) + 1 ) & 0xFF;

    return(&ints[0]);
    /*TODO
    ints = [0,0,0,0]
    ints[0] = (seq) | (((seq+1)%8) << 3) | (dicp << 6) | (rel << 7)
    ints[1] = pkt_type | ((pkt_len & 0x000F) << 4)
    ints[2] = (pkt_len & 0x0FF0) >> 4
    ints[3] = (~(sum(ints[0:3]))+1) & 0xFF

    return ''.join(chr(b) for b in ints)
    */
}

static uint8_t * bytePtr_int32_to_bytes(uint32_t nr)
{
    static uint8_t ints[4];
    ints[0] = (nr & 0x000000FF) ;
    ints[1] = (nr & 0x0000FF00) >> 8;
    ints[2] = (nr & 0x00FF0000) >> 16;
    ints[3] = (nr & 0xFF000000) >> 24;
    return(ints);
    //return ''.join(chr(b) for b in ints)
}

static uint8_t * bytePtr_int16_to_bytes(uint32_t nr)
{
    static uint8_t ints[2];
    ints[0] = (nr & 0x000000FF) ;
    ints[1] = (nr & 0x0000FF00) >> 8;
    return(ints);
}
//def G_int16_to_bytes(nr):
//    ints = [0,0]
//    ints[0] = (nr & 0x00FF) 
//    ints[1] = (nr & 0xFF00) >> 8
//    return ''.join(chr(b) for b in ints)     


extern "C" uint16_t crc16_compute(uint8_t * p_data, uint16_t size, uint16_t * p_crc);

extern "C" uint8_t slip_encode_byte( uint8_t u8, uint8_t* b0, uint8_t* b1 );

uint8_t slip_encode_byte( uint8_t bIn, uint8_t* b0, uint8_t* b1 )
{
    // Replace 0xCO  with 0xDBDC and 0xDB with 0xDBDD
    
    if(bIn == 0xC0)
    {
        *b0 = 0xDB;
        *b1 = 0xDC;
    }
    else
    {
        if(bIn == 0xDB)
        {
            *b0 = 0xDB;
            *b1 = 0xDD;
        }
        else
        {
            *b0 = bIn;
            *b1 = 0;
            return(1);
        }
    }
    return(2);
}

#define DATA_INTEGRITY_CHECK_PRESENT   1
#define RELIABLE_PACKET    1
#define HCI_PACKET_TYPE   14

#define DFU_INIT_PACKET       1
#define DFU_START_PACKET      3
#define DFU_DATA_PACKET       4
#define DFU_END_PACKET        5 //DFU_STOP_DATA_PACKET
#define DFU_STOP_DATA_PACKET  5 //DFU_END_PACKET

#define DFU_UPDATE_MODE_NONE    0
#define DFU_UPDATE_MODE_SD      1
#define DFU_UPDATE_MODE_BL      2
#define DFU_UPDATE_MODE_APP     4


struct _HciPacket
{
    uint32_t sequence_number;//TODO = 0;
    //buf32_t temp_data;
} W_HciPacket;

extern "C" void W_HciPacket_ZERO(void);
void W_HciPacket_ZERO(void)
{
     W_HciPacket.sequence_number = 0;
}

//#include "memory.h"
//#include "stdlib.h" //malloc
int32_t make_HciPacket_cb16(uint8_t* pDin, uint16_t lenDin, cb16_t* cbOut, uint16_t* plenDout )
{
    uint16_t  totalLength;
    uint16_t  crc;

    uint16_t  i;

    uint8_t*  pdata4;
    uint8_t len12;
    uint8_t b0;
    uint8_t b1;
    uint8_t  crc01[2];


    if( cbOut != 0) 
        W_HciPacket.sequence_number = (W_HciPacket.sequence_number+1) % 8;

    pdata4 = bytePtr_parts_to_four_bytes( W_HciPacket.sequence_number, 
                                          DATA_INTEGRITY_CHECK_PRESENT,
                                          RELIABLE_PACKET,
                                          HCI_PACKET_TYPE,
                                          lenDin );

    crc = 0xFFFF;
    crc = crc16_compute( pdata4, 4     , &crc);
    crc = crc16_compute( pDin  , lenDin, &crc);

    crc01[0] = (uint8_t)( crc & 0xFF);
    crc01[1] = (uint8_t)((crc & 0xFF00) >> 8);
               
    totalLength = 0;

    if( cbOut != 0) cb16_push( cbOut, 0xC0  );
    totalLength += 1;

    for( i=0 ; i<4; i++ )
    {
        len12 = slip_encode_byte( pdata4[i], &b0, &b1 );
        if( cbOut != 0) cb16_push( cbOut, b0 );
        totalLength += 1;
        if(len12 == 2)
        {
            if( cbOut != 0) cb16_push( cbOut, b0 );
            totalLength += 1;
        }
    }

    for( i=0 ; i<lenDin; i++ )
    {
        len12 = slip_encode_byte( pDin[i], &b0, &b1 );
        if( cbOut != 0) cb16_push( cbOut, b0 );
        totalLength += 1;
        if(len12 == 2)
        {
            if( cbOut != 0) cb16_push( cbOut, b0 );
            totalLength += 1;
        }
    }

    for( i=0 ; i<2; i++ )
    {
        len12 = slip_encode_byte( crc01[i], &b0, &b1 );
        if( cbOut != 0) cb16_push( cbOut, b0 );
        totalLength += 1;
        if(len12 == 2)
        {
            if( cbOut != 0) cb16_push( cbOut, b0 );
            totalLength += 1;
        }
    }

    if( cbOut != 0) cb16_push( cbOut, b0 );
    totalLength += 1;

    *plenDout = totalLength;

    return(totalLength);
}

#if 1
uint8_t gx_ints[4];
buf32_t gx_ret_buf600;

buf32_t* buf32_parts_to_four_bytes(uint32_t seq, uint32_t dicp, uint32_t rel, uint32_t pkt_type, uint32_t pkt_len)
{
    uint8_t ints[4];

    gx_ret_buf600.buffer = gx_ints;
    buf32_zero(&gx_ret_buf600);
    ints[0] = 0;  ints[1] = 0;  ints[2] = 0;  ints[3] = 0;

    ints[0] = (seq) | (((seq+1)%8) << 3) | (dicp << 6) | (rel << 7);
    ints[1] = pkt_type | ((pkt_len & 0x000F) << 4);
    ints[2] = (pkt_len & 0x0FF0) >> 4;
    ints[3] = ( ~(ints[0]+ints[1]+ints[2]+ints[3]) + 1 ) & 0xFF;

    gx_ret_buf600.buffer[0] = ints[0];
    gx_ret_buf600.buffer[1] = ints[1];
    gx_ret_buf600.buffer[2] = ints[2];
    gx_ret_buf600.buffer[3] = ints[3];
    gx_ret_buf600.length = 4;
    return(&gx_ret_buf600);
    /*TODO
    ints = [0,0,0,0]
    ints[0] = (seq) | (((seq+1)%8) << 3) | (dicp << 6) | (rel << 7)
    ints[1] = pkt_type | ((pkt_len & 0x000F) << 4)
    ints[2] = (pkt_len & 0x0FF0) >> 4
    ints[3] = (~(sum(ints[0:3]))+1) & 0xFF

    return ''.join(chr(b) for b in ints)
    */
}
#endif
static buf32_t* buf32_slip_encode_packet(buf32_t *data_in)
{
    // Replace 0xCO  with 0xDBDC and 0xDB with 0xDBDD
    static uint8_t resultbuf[2 * (4 + 4+512 + 2)];
    static buf32_t result32;
    buf32_t *result;
    uint32_t i;
    uint32_t j;
    uint8_t _char;

    result32.buffer = resultbuf;
    result32.capacity = 2 * (4 + 4+512 + 2);

    result = &result32;


    buf32_zero(result);
    j=0;

    for( i=0; i<data_in->length; i++)
    {
        _char = data_in->buffer[i];
        if(_char == 0xC0)
        {
            result->buffer[j++] = 0xDB;
            result->buffer[j++] = 0xDC;
        }
        else
            if(_char == 0xDB)
            {
                result->buffer[j++] = 0xDB;
                result->buffer[j++] = 0xDD;
            }
            else
            {
                result->buffer[j++] = _char;
            }
    }
    result->length = j;
    return(result);

    /*
    //result = []
    //data = []
    for i in data_in:
        data.append(ord(i))
        
    while len(data):
        _char = data.pop(0)
        if _char == 0xC0:
            result.extend([0xDB, 0xDC])
        elif _char == 0xDB:
            result.extend([0xDB, 0xDD])
        else:
            result.append(_char)
    return ''.join(chr(i) for i in result)
    */
}

//*****************************************************************************
//*****************************************************************************
//*****************************************************************************



static struct localW_slip_HciPacket_s
{
    uint32_t sequence_number;//TODO = 0;
    //buf32_t temp_data;
} HciPacket;

void malloc_HciPacket_Set_sequence_number(uint32_t sequence_number)
{
    HciPacket.sequence_number = sequence_number;
}

buf32_t* malloc_HciPacket(buf32_t* pFrame)
{
    uint16_t crc;
    buf32_t *pEncoded_STATIC;
    buf32_t *pSlipPkt;
    buf32_t *pTemp;

    static uint8_t _temp_buffer[4 + 4+512 + 2];
    static buf32_t _temp;
    
    _temp.buffer = _temp_buffer;
    _temp.capacity =  sizeof(_temp_buffer ); //4 + 4+512 + 2;
    pTemp = &_temp;

    //----------------------
    //    Add HCI Header
    //----------------------
    HciPacket.sequence_number = (HciPacket.sequence_number+1) % 8;
    buf32_zero(pTemp);
    buf32_join(pTemp, buf32_parts_to_four_bytes( HciPacket.sequence_number, 
                                                 DATA_INTEGRITY_CHECK_PRESENT,
                                                 RELIABLE_PACKET,
                                                 HCI_PACKET_TYPE,
                                                 pFrame->length) );
    //buf32_print("## 0", pTemp);

    //-------------------
    //    Add payload 
    //-------------------
    buf32_join(pTemp, pFrame);
    //// Add escape caracters
    //buf32_print("## 1", pTemp);

    //---------------
    //    Add CRC    
    //---------------
    crc = 0xFFFF;
    crc = crc16_compute(pTemp->buffer, pTemp->length, &crc);

    pTemp->buffer[pTemp->length] = (uint8_t)(crc & 0xFF);          pTemp->length++; //chr(crc & 0xFF)
    pTemp->buffer[pTemp->length] = (uint8_t)((crc & 0xFF00) >> 8); pTemp->length++; //chr((crc & 0xFF00) >> 8)
    //buf32_print("## 2", pTemp);


    //------------------------
    //    Do SLIP encoding
    //------------------------
    pEncoded_STATIC = buf32_slip_encode_packet(pTemp);
    //buf32_print("## pEncoded_STATIC", pEncoded_STATIC);


    //---------------------------------
    //    Add 0xC0 to start and end
    //---------------------------------
    pSlipPkt = buf32_Create(1 + pEncoded_STATIC->length + 1);

    //buf32_zero(pSlipPkt);    
    pSlipPkt->length = 0;
    pSlipPkt->buffer[pSlipPkt->length] = 0xC0;  //self->buffer = chr(0xc0)
    pSlipPkt->length++;
    //buf32_print("## pSlipPkt", pSlipPkt);

    buf32_join(pSlipPkt, pEncoded_STATIC);             //self->buffer += self.temp_data
    //buf32_print("## pSlipPkt", pSlipPkt);
    
    pSlipPkt->buffer[pSlipPkt->length] = 0xC0;  //self->buffer += chr(0xc0)
    pSlipPkt->length++;
    //buf32_print("## pSlipPkt", pSlipPkt);

    return(pSlipPkt);
}


buf32_t* malloc_frame_DFU_START_PACKET(int32_t program_mode, int32_t softdevice_size, int32_t bootloader_size, int32_t app_size)
{
    buf32_t *frame = 0;

    /*
    uint32_t old_sd_size;
    uint32_t old_bl_size;
    uint32_t old_app_size;
    old_sd_size = word_align(pih->sd_address_end, 4) - pih->sd_address_start);
    old_bl_size = word_align(pih->bl_address_end, 4) - pih->bl_address_start);
    old_app_size = word_align(pih->app_address_end, 4) - pih->app_address_start);
    */
    frame = buf32_Create(20);

    buf32_zero(frame);
    buf32_cpy(frame, int32_to_bytes(DFU_START_PACKET), 4);
    buf32_cpy(frame, int32_to_bytes(program_mode), 4);      //hex_type    
    buf32_cpy(frame, int32_to_bytes(softdevice_size), 4 );  // size of SoftDevice    
    buf32_cpy(frame, int32_to_bytes(bootloader_size), 4 );  // size of BootLoader
    buf32_cpy(frame, int32_to_bytes(app_size), 4 );         // size of Application


    buf32_print("malloc_frame_DFU_START_PACKET", frame);

    return( frame );
}


buf32_t* malloc_frame_DFU_INIT_PACKET(buf32_t* init_packet)
{
    buf32_t *frame = 0;

    /*
    printf("DfuTransportSerial->send_init_packet()\n");
    printf("    init_packet length = %d\n", init_packet->length );
    */
    uint16_t hardware_version    = *(uint16_t*)&init_packet->buffer[0]; //0x0000 //0xFFFF;
    uint16_t hardware_revision   = *(uint16_t*)&init_packet->buffer[2]; //0x0000; //0xFFFF;
    uint32_t application_version = *(uint32_t*)&init_packet->buffer[4]; //0x00000000; //0xFFFFFFFF;
    uint16_t softdevice_len      = *(uint16_t*)&init_packet->buffer[8]; //0x0001;
    uint16_t softdevice_array    = *(uint16_t*)&init_packet->buffer[10]; //0xFFFE;
    uint16_t image_crc           = *(uint16_t*)&init_packet->buffer[12];
    //uint16_t pad16               = *(uint16_t*)&init_packet->buffer[14];
    
    /*
    printf("     hardware_version    = 0x%04x\n", hardware_version);
    printf("     hardware_revision   = 0x%04x\n", hardware_revision);
    printf("     application_version = 0x%08x\n", application_version);
    printf("     softdevice_len      = 0x%04x\n", softdevice_len);
    printf("     softdevice_array    = 0x%04x\n", softdevice_array);
    printf("     image_crc           = 0x%04x\n", image_crc);
    //printf("     pad16               = 0x%04x\n", pad16);
    printf("\n");
    */

    frame = buf32_Create(20);
    buf32_zero(frame);

    buf32_cpy(frame, int32_to_bytes(DFU_INIT_PACKET), 4);
    buf32_cpy(frame, int16_to_bytes(hardware_version), 2);
    buf32_cpy(frame, int16_to_bytes(hardware_revision), 2);
    buf32_cpy(frame, int32_to_bytes(application_version), 4);
    buf32_cpy(frame, int16_to_bytes(softdevice_len), 2);
    buf32_cpy(frame, int16_to_bytes(softdevice_array), 2);
    buf32_cpy(frame, int16_to_bytes(image_crc), 2);
    buf32_cpy(frame, int16_to_bytes(0x0000), 2);

    buf32_print("malloc_frame_DFU_INIT_PACKET", frame);

    return( frame );


    /*REF
    init_packet = int32_to_bytes(DFU_INIT_PACKET);
    init_packet += int16_to_bytes(hardware_version);
    init_packet += int16_to_bytes(hardware_revision);
    init_packet += int32_to_bytes(application_version);
    init_packet += int16_to_bytes(softdevice_len);
    init_packet += int16_to_bytes(softdevice_array);
    init_packet += int16_to_bytes(image_crc);
    init_packet += int16_to_bytes(0x0000);
    REF*/
}

buf32_t* malloc_frame_DFU_DATA_PACKET(uint8_t* buffer, uint32_t length)
{
    //data_packet = HciPacket(int32_to_bytes(DFU_DATA_PACKET) + firmware[i:i + DFU_PACKET_MAX_SIZE])
    buf32_t *frame = 0;

    frame = buf32_Create(4 + length);
    buf32_zero(frame);

    buf32_cpy(frame, int32_to_bytes(DFU_DATA_PACKET), 4);
    buf32_cpy(frame, buffer, length);

    buf32_printShort("malloc_DFU_DATA_PACKET_frame", frame);

    return( frame );
}

buf32_t* malloc_frame_DFU_STOP_DATA_PACKET(void) //DFU_END_PACKET
{
    buf32_t *frame = 0;

    frame = buf32_Create(4);
    buf32_zero(frame);

    buf32_cpy(frame, int32_to_bytes(DFU_STOP_DATA_PACKET), 4); //DFU_END_PACKET

    buf32_print("malloc_frame_DFU_STOP_DATA_PACKET", frame);

    return( frame );
}
