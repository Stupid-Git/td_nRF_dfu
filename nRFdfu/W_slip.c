#include "W_slip.h"



//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
#include <memory.h>
#include <stdio.h>


//*****************************************************************************
//*****************************************************************************
//*****************************************************************************

uint8_t* W_G_parts_to_four_bytes(uint32_t seq, uint32_t dicp, uint32_t rel, uint32_t pkt_type, uint32_t pkt_len)
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

uint8_t * W_int32_to_bytes(uint32_t nr)
{
    static uint8_t ints[4];
    ints[0] = (nr & 0x000000FF) ;
    ints[1] = (nr & 0x0000FF00) >> 8;
    ints[2] = (nr & 0x00FF0000) >> 16;
    ints[3] = (nr & 0xFF000000) >> 24;
    return(ints);
    //return ''.join(chr(b) for b in ints)
}

uint8_t * W_int16_to_bytes(uint32_t nr)
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

#if 0 //RMRMRM

buf32_t * W_G_decode_esc_chars(buf32_t * data)
{

    //'''Replace 0xDBDC with 0xCO and 0xDBDD with 0xDB'''
    uint32_t _len;
    uint8_t _char;
    uint8_t _char2;
    buf32_t * result; // = []
    static buf32_t XXX;
    uint8_t _char_C0;
    uint8_t _char_DB;
    uint32_t pos;

    _char_C0 = 0xC0;
    _char_DB = 0xDB;

    result = &XXX;
    buf32_zero(result);

    pos = 0;
    _len = data->len;
    while( pos < _len)
    {
        _char = data->data[pos++]; //data.pop(0);
        if(_char == 0xDB)
        {
            _char2 = data->data[pos++]; //data.pop(0);
            if(_char2 == 0xDC)
                buf32_cpy(result, &_char_C0, 1);  //result.append(0xC0);
            else
                if(_char2 == 0xDD)
                    buf32_cpy(result, &_char_DB, 1);  //result.append(0xDB);
                else
                {
                    //raise Exception('Char 0xDB NOT followed by 0xDC or 0xDD');
                    printf("G_decode_esc_chars: Exception: Char 0xDB NOT followed by 0xDC or 0xDD \n");
                    return(0);
                }
        }
        else
        {
            buf32_cpy(result, &_char, 1);  //result.append(_char);
        }
    }
    return result;
    /*
    def G_decode_esc_chars(data):
        '''Replace 0xDBDC with 0xCO and 0xDBDD with 0xDB'''
        result = []
        while len(data):
            char = data.pop(0)
            if char == 0xDB:
                char2 = data.pop(0)
                if char2 == 0xDC:
                    result.append(0xC0)
                elif char2 == 0xDD:
                    result.append(0xDB)
                else:
                    raise Exception('Char 0xDB NOT followed by 0xDC or 0xDD')
            else:
                result.append(char)
        return result
    */ 
}


#endif //0 //RMRMRM

extern "C" uint16_t crc16_compute(uint8_t * p_data, uint16_t size, uint16_t * p_crc);

extern "C" uint8_t SLIP_EncodeByte( uint8_t u8, uint8_t* b0, uint8_t* b1 );

uint8_t SLIP_EncodeByte( uint8_t bIn, uint8_t* b0, uint8_t* b1 )
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

    pdata4 = W_G_parts_to_four_bytes( W_HciPacket.sequence_number, 
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
        len12 = SLIP_EncodeByte( pdata4[i], &b0, &b1 );
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
        len12 = SLIP_EncodeByte( pDin[i], &b0, &b1 );
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
        len12 = SLIP_EncodeByte( crc01[i], &b0, &b1 );
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


uint8_t gx_ints[4];
buf32_t gx_ret_buf600;

buf32_t* G_parts_to_four_bytes(uint32_t seq, uint32_t dicp, uint32_t rel, uint32_t pkt_type, uint32_t pkt_len)
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

static buf32_t* _encode_packet(buf32_t *data_in)
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


// C0
//   D1 4E 01 E0 
//     03 00 00 00 int32_to_bytes(DFU_START_PACKET);
//     04 00 00 00 int32_to_bytes(hex_type);
//     00 00 00 00 int32_to_bytes(word_align(hex_file.sd_address_end, 4) - hex_file.sd_address_start) ;
//     00 00 00 00 int32_to_bytes(word_align(hex_file.bl_address_end, 4) - hex_file.bl_address_start) ;
//     CC 27 00 00 int32_to_bytes(word_align(hex_file.app_address_end, 4) - hex_file.app_address_start) ;
//   AB 50
// C0


// C0 
//   DA 4E 01 D7 
//      01 00 00 00 
//      00 00 00 00 
//      00 00 00 00 
//      01 00 FE FF 
//      F2 0F 00 00
//   62 6D
// C0  

/*
// C0 
//   E3 4E 20 AF 
//      04 00 00 00 
           28 2E 00 20 F5 80 01 00 0F 81 01 00 11 81 01 00 
           00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
           00 00 00 00 00 00 00 00 00 00 00 00 13 81 01 00 
           00 00 00 00 00 00 00 00 15 81 01 00 17 81 01 00 
           19 81 01 00 19 81 01 00 AD 83 01 00 19 81 01 00 
           19 81 01 00 00 00 00 00 C5 81 01 00 19 81 01 00 
           19 81 01 00 19 81 01 00 19 81 01 00 19 81 01 00 
           19 81 01 00 19 81 01 00 19 81 01 00 19 81 01 00 
           19 81 01 00 C1 82 01 00 19 81 01 00 19 81 01 00 
           E1 82 01 00 19 81 01 00 35 83 01 00 19 81 01 00 
           19 81 01 00 19 81 01 00 00 00 00 00 00 00 00 00 
           00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
           03 48 85 46 00 F0 6C F8 00 48 00 47 1D 9A 01 00 
           28 2E 00 20 40 1E 00 BF 00 BF 00 BF 00 BF 00 BF 
           00 BF 00 BF 00 BF 00 BF 00 BF 00 BF 00 BF F1 D1 
           70 47 00 00 03 21 09 48 02 68 0A 43 02 60 08 48 
           02 68 0A 43 02 60 07 48 80 47 07 48 00 47 FE E7 
           FE E7 FE E7 FE E7 FE E7 FE E7 00 00 24 05 00 40 
           54 05 00 40 59 83 01 00 C1 80 01 00 30 B5 0B 46 
           01 46 00 20 20 22 01 24 09 E0 0D 46 D5 40 9D 42 
           05 D3 1D 46 95 40 49 1B 25 46 95 40 40 19 15 46 
           52 1E 00 2D F1 DC 30 BD 03 46 0B 43 9B 07 03 D0  
           09 E0 08 C9 12 1F 08DBDC04 2A FA D2 03 E0 0B 78 ** 
           03 70 40 1C 49 1C 52 1E F9 D2 70 47 D2 B2 01 E0
           02 70 40 1C 49 1E FB D2 70 47 00 22 F6 E7 10 B5 
           13 46 0A 46 04 46 19 46 FF F7 F0 FF 20 46 10 BD
           06 4C 01 25 06 4E 05 E0 E3 68 07 CC 2B 43 0C 3C  
           98 47 10 34 B4 42 F7 D3 FF F7 86 FF FC A6 01 00 
           1C A7 01 00 F0 B5 2A 48 85 B0 04 69 29 48 00 21 
           C1 63 0E 46 28 4F 45 E0 14 20 79 68 70 43 45 18 
           01 20 B9 68 B0 40 08 42 3A D0 00 20 00 90 03 90
           E8 68 29 68DBDC43 60 40 08 40 01 46 02 90 28 46 ** 
//   7B 5F 
//C0  
*/

/*
#### Controller_BOGUS_data_packet_generate ####
//c0
//    e3 4e 20 af 
//    04 00 00 00 
           28 2e 00 20 f5 80 01 00 0f 81 01 00 11 81 01 00
           00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
           00 00 00 00 00 00 00 00 00 00 00 00 13 81 01 00 
           00 00 00 00 00 00 00 00 15 81 01 00 17 81 01 00 
           19 81 01 00 19 81 01 00 ad 83 01 00 19 81 01 00 
           19 81 01 00 00 00 00 00 c5 81 01 00 19 81 01 00 
           19 81 01 00 19 81 01 00 19 81 01 00 19 81 01 00 
           19 81 01 00 19 81 01 00 19 81 01 00 19 81 01 00 
           19 81 01 00 c1 82 01 00 19 81 01 00 19 81 01 00 
           e1 82 01 00 19 81 01 00 35 83 01 00 19 81 01 00 
           19 81 01 00 19 81 01 00 00 00 00 00 00 00 00 00 
           00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
           03 48 85 46 00 f0 6c f8 00 48 00 47 1d 9a 01 00 
           28 2e 00 20 40 1e 00 bf 00 bf 00 bf 00 bf 00 bf 
           00 bf 00 bf 00 bf 00 bf 00 bf 00 bf 00 bf f1 d1 
           70 47 00 00 03 21 09 48 02 68 0a 43 02 60 08 48 
           02 68 0a 43 02 60 07 48 80 47 07 48 00 47 fe e7 
           fe e7 fe e7 fe e7 fe e7 fe e7 00 00 24 05 00 40 
           54 05 00 40 59 83 01 00 c1 80 01 00 30 b5 0b 46 
           01 46 00 20 20 22 01 24 09 e0 0d 46 d5 40 9d 42 
           05 d3 1d 46 95 40 49 1b 25 46 95 40 40 19 15 46 
           52 1e 00 2d f1 dc 30 bd 03 46 0b 43 9b 07 03 d0 
           09 e0 08 c9 12 1f 08 db dc 04 2a fa d2 03 e0 0b 78 ** 
           03 70 40 1c 49 1c 52 1e f9 d2 70 47 d2 b2 01 e0 
           02 70 40 1c 49 1e fb d2 70 47 00 22 f6 e7 10 b5 
           13 46 0a 46 04 46 19 46 ff f7 f0 ff 20 46 10 bd 
           06 4c 01 25 06 4e 05 e0 e3 68 07 cc 2b 43 0c 3c 
           98 47 10 34 b4 42 f7 d3 ff f7 86 ff fc a6 01 00 
           1c a7 01 00 f0 b5 2a 48 85 b0 04 69 29 48 00 21 
           c1 63 0e 46 28 4f 45 e0 14 20 79 68 70 43 45 18 
           01 20 b9 68 b0 40 08 42 3a d0 00 20 00 90 03 90 
           e8 68 29 68 db dc 43 60 40 08 40 01 46 02 90 28 46 **
//   ad de 
//c0
packet->len = 526
*/


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
    buf32_join(pTemp, G_parts_to_four_bytes(HciPacket.sequence_number, 
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
    pEncoded_STATIC = _encode_packet(pTemp);
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

#if 0 // JUNK

uint8_t GdummyData[512] = {
    0x28, 0x2E , 0x00, 0x20 , 0xF5 , 0x80 , 0x01 , 0x00 , 0x0F , 0x81 , 0x01 , 0x00 , 0x11 , 0x81 , 0x01 , 0x00 , //
    0x00, 0x00 , 0x00, 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , //
    0x00, 0x00 , 0x00, 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x13 , 0x81 , 0x01 , 0x00 , //
    0x00, 0x00 , 0x00, 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x15 , 0x81 , 0x01 , 0x00 , 0x17 , 0x81 , 0x01 , 0x00 , //
    0x19, 0x81 , 0x01, 0x00 , 0x19 , 0x81 , 0x01 , 0x00 , 0xAD , 0x83 , 0x01 , 0x00 , 0x19 , 0x81 , 0x01 , 0x00 , //
    0x19, 0x81 , 0x01, 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0xC5 , 0x81 , 0x01 , 0x00 , 0x19 , 0x81 , 0x01 , 0x00 , //
    0x19, 0x81 , 0x01, 0x00 , 0x19 , 0x81 , 0x01 , 0x00 , 0x19 , 0x81 , 0x01 , 0x00 , 0x19 , 0x81 , 0x01 , 0x00 , //
    0x19, 0x81 , 0x01, 0x00 , 0x19 , 0x81 , 0x01 , 0x00 , 0x19 , 0x81 , 0x01 , 0x00 , 0x19 , 0x81 , 0x01 , 0x00 , //
    0x19, 0x81 , 0x01, 0x00 , 0xC1 , 0x82 , 0x01 , 0x00 , 0x19 , 0x81 , 0x01 , 0x00 , 0x19 , 0x81 , 0x01 , 0x00 , //
    0xE1, 0x82 , 0x01, 0x00 , 0x19 , 0x81 , 0x01 , 0x00 , 0x35 , 0x83 , 0x01 , 0x00 , 0x19 , 0x81 , 0x01 , 0x00 , //
    0x19, 0x81 , 0x01, 0x00 , 0x19 , 0x81 , 0x01 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , //
    0x00, 0x00 , 0x00, 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , //
    0x03, 0x48 , 0x85, 0x46 , 0x00 , 0xF0 , 0x6C , 0xF8 , 0x00 , 0x48 , 0x00 , 0x47 , 0x1D , 0x9A , 0x01 , 0x00 , //
    0x28, 0x2E , 0x00, 0x20 , 0x40 , 0x1E , 0x00 , 0xBF , 0x00 , 0xBF , 0x00 , 0xBF , 0x00 , 0xBF , 0x00 , 0xBF , //
    0x00, 0xBF , 0x00, 0xBF , 0x00 , 0xBF , 0x00 , 0xBF , 0x00 , 0xBF , 0x00 , 0xBF , 0x00 , 0xBF , 0xF1 , 0xD1 , //
    0x70, 0x47 , 0x00, 0x00 , 0x03 , 0x21 , 0x09 , 0x48 , 0x02 , 0x68 , 0x0A , 0x43 , 0x02 , 0x60 , 0x08 , 0x48 , //
    0x02, 0x68 , 0x0A, 0x43 , 0x02 , 0x60 , 0x07 , 0x48 , 0x80 , 0x47 , 0x07 , 0x48 , 0x00 , 0x47 , 0xFE , 0xE7 , //
    0xFE, 0xE7 , 0xFE, 0xE7 , 0xFE , 0xE7 , 0xFE , 0xE7 , 0xFE , 0xE7 , 0x00 , 0x00 , 0x24 , 0x05 , 0x00 , 0x40 , //
    0x54, 0x05 , 0x00, 0x40 , 0x59 , 0x83 , 0x01 , 0x00 , 0xC1 , 0x80 , 0x01 , 0x00 , 0x30 , 0xB5 , 0x0B , 0x46 , //
    0x01, 0x46 , 0x00, 0x20 , 0x20 , 0x22 , 0x01 , 0x24 , 0x09 , 0xE0 , 0x0D , 0x46 , 0xD5 , 0x40 , 0x9D , 0x42 , //
    0x05, 0xD3 , 0x1D, 0x46 , 0x95 , 0x40 , 0x49 , 0x1B , 0x25 , 0x46 , 0x95 , 0x40 , 0x40 , 0x19 , 0x15 , 0x46 , //
    0x52, 0x1E , 0x00, 0x2D , 0xF1 , 0xDC , 0x30 , 0xBD , 0x03 , 0x46 , 0x0B , 0x43 , 0x9B , 0x07 , 0x03 , 0xD0 , // 
    0x09, 0xE0 , 0x08, 0xC9 , 0x12 , 0x1F , 0x08 , 0xC0 , 0x04 , 0x2A , 0xFA , 0xD2 , 0x03 , 0xE0 , 0x0B , 0x78 , //** 
    0x03, 0x70 , 0x40, 0x1C , 0x49 , 0x1C , 0x52 , 0x1E , 0xF9 , 0xD2 , 0x70 , 0x47 , 0xD2 , 0xB2 , 0x01 , 0xE0 , //
    0x02, 0x70 , 0x40, 0x1C , 0x49 , 0x1E , 0xFB , 0xD2 , 0x70 , 0x47 , 0x00 , 0x22 , 0xF6 , 0xE7 , 0x10 , 0xB5 , //
    0x13, 0x46 , 0x0A, 0x46 , 0x04 , 0x46 , 0x19 , 0x46 , 0xFF , 0xF7 , 0xF0 , 0xFF , 0x20 , 0x46 , 0x10 , 0xBD , //
    0x06, 0x4C , 0x01, 0x25 , 0x06 , 0x4E , 0x05 , 0xE0 , 0xE3 , 0x68 , 0x07 , 0xCC , 0x2B , 0x43 , 0x0C , 0x3C , // 
    0x98, 0x47 , 0x10, 0x34 , 0xB4 , 0x42 , 0xF7 , 0xD3 , 0xFF , 0xF7 , 0x86 , 0xFF , 0xFC , 0xA6 , 0x01 , 0x00 , //
    0x1C, 0xA7 , 0x01, 0x00 , 0xF0 , 0xB5 , 0x2A , 0x48 , 0x85 , 0xB0 , 0x04 , 0x69 , 0x29 , 0x48 , 0x00 , 0x21 , //
    0xC1, 0x63 , 0x0E, 0x46 , 0x28 , 0x4F , 0x45 , 0xE0 , 0x14 , 0x20 , 0x79 , 0x68 , 0x70 , 0x43 , 0x45 , 0x18 , //
    0x01, 0x20 , 0xB9, 0x68 , 0xB0 , 0x40 , 0x08 , 0x42 , 0x3A , 0xD0 , 0x00 , 0x20 , 0x00 , 0x90 , 0x03 , 0x90 , //
    0xE8, 0x68 , 0x29, 0x68 , 0xC0 , 0x43 , 0x60 , 0x40 , 0x08 , 0x40 , 0x01 , 0x46 , 0x02 , 0x90 , 0x28 , 0x46   //** 
};

buf32_t * Controller_BOGUS_data_packet_generate()
{
    buf32_t *packet = 0;
    buf32_t data_packet;
    
    buf32_zero(&data_packet);

    buf32_cpy(&data_packet, int32_to_bytes(DFU_DATA_PACKET), 4);
    buf32_cpy(&data_packet, GdummyData, 512);

    //buf32_print("#### Controller_BOGUS_data_packet_generate ####", &data_packet);
    //buf32_printShort("#### Controller_BOGUS_data_packet_generate ####", &data_packet);
    packet = malloc_HciPacket(&data_packet); //int32_to_bytes(DFU_DATA_PACKET) + bin_image[i:i+512]);
    buf32_print("\n#### Controller_BOGUS_data_packet_generate ####", packet);
    buf32_printShort("\n#### Controller_BOGUS_data_packet_generate ####", packet);
    
    
    return(packet);

    //def word_align(a, b):
    //    return (((a) + (b - 1)) &~(b - 1))
    //foo_packets = []  
    /*TODO
    for i in range(0, len(bin_image), 512)
    {
        data_packet = malloc_HciPacket(int32_to_bytes(DFU_DATA_PACKET) + bin_image[i:i+512])

            // print "\n\ndata packet data:",
            // for i in data_packet->buffer:
            //     print hex(ord(i)),

            foo_packets.append(data_packet)
    }
    return foo_packets;
    */
}

#endif //0 // JUNK
