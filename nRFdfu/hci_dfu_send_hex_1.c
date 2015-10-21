
#include "stdint.h"
#include "stdio.h"
#include "memory.h"

#include "../src/stdbool.h"

//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
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

#include "../src/myutils.h"
#include "W_slip.h"



//class Controller(object):
char *m_Controller_file_path = 0;
char *gx_Controller_com_port = 0;//None;
int   gx_Controller_flow_control = 0;//None;
static int   m_Controller_baud_rate = 0;//None;
int   gx_Controller_verbose = 0; //None;
int   gx_Controller_progress_value = -1;

typedef struct packetList_s 
{
    buf32_t **packet;
    int pktCount;
    int Capacity;
} packetList_t;
static packetList_t m_packetList;

//#include "memory.h"
#include "stdlib.h" //malloc

int packetList_CreateEntries(packetList_t* p_packetList, int packetCount)
{
    int i;
    p_packetList->packet = (buf32_t**)calloc(packetCount, sizeof(buf32_t**) );
    p_packetList->Capacity = packetCount;
    p_packetList->pktCount = 0;

    for( i=0 ; i< packetCount; i++)
        p_packetList->packet[i] = 0;

    return(packetCount);
}

void  packetList_DestroyEntries(packetList_t packetList)
{
    int i;
    int packetCount;

    packetCount = packetList.Capacity;

    for( i=0 ; i< packetCount; i++)
    {
        if( packetList.packet[i] != 0 )
            free(packetList.packet[i]);
    }
    free(packetList.packet);
    packetList.Capacity = 0;
    packetList.pktCount = 0;

}


#include "W_Nrf51Hex.h"
static nRFhex_t m_NRFHEX_BOGUS;

#include "W_ihexLoadToMem.h"

#define GXIMAGE_BUFFER_CAPACITY 0x8000
static uint8_t m_gxImage_buffer[GXIMAGE_BUFFER_CAPACITY];
static W_ihexMemImage_t m_gxImage0;
static W_ihexMemImage_t* m_gxImage;


#include "../src/UartSerial/uart_xx.h"

uint8_t * int32_to_bytes(uint32_t nr);
uint32_t word_align(uint32_t a, uint32_t b);
uint8_t * int16_to_bytes(uint32_t nr);

//*****************************************************************************
//*****************************************************************************
//*****************************************************************************

#define DEFAULT_SERIAL_PORT_TIMEOUT  1.0  //# Timeout time on serial port read
#define ACK_PACKET_TIMEOUT           1.0  //# Timeout time for for ACK packet received before reporting timeout through event system
#define SEND_INIT_PACKET_WAIT_TIME   1.0  //# Time to wait before communicating with bootloader after init packet is sent
#define SEND_START_DFU_WAIT_TIME    10.0  //# Time to wait before communicating with bootloader after start DFU packet is sent
#define DFU_PACKET_MAX_SIZE        512    //# The DFU packet max size


//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
void Controller_init()
{
    m_Controller_file_path = 0;
    gx_Controller_com_port = 0;//None;
    gx_Controller_flow_control = 0;//None;
    m_Controller_baud_rate = 0;//None;
    gx_Controller_verbose = 0;//None;
    gx_Controller_progress_value = -1;
}        

void Controller_progress_callback(int value, int bar_length/*=30*/)
{
    if(value != gx_Controller_progress_value)
    {
        gx_Controller_progress_value = value;
        /*TODO
        hashes = '//' * int((float(Controller_progress_value)/100) * bar_length)
        spaces = ' ' * (bar_length - len(hashes))
        sys.stdout.write("\rProgress: [{0}] {1}%".format(hashes + spaces, Controller_progress_value))
        sys.stdout.flush()
        */
        printf("Progress: %d\n", gx_Controller_progress_value );
    }
}

void Controller_timeout_callback()
{
    printf("Transmission timeout.\n");
}

void Controller_set_filepath(char* filepath)
{
    m_Controller_file_path = filepath;
    /*TODO
    if not os.path.isfile(os.path.abspath(m_Controller_file_path))
    {
        print "File does not exist: %s" % os.path.abspath(m_Controller_file_path)
            sys.exit(1)
    }
    */
}
      
void Controller_upload_file()
{
    /*TODO
    if(m_Controller_file_path == 0)
        raise Exception("File path not set, or not found.")

    s = ThreadedFunction(self.Controller_upload_firmware)
    s.start()
    */
}

// Verbose summary
void Controller_verbose_info(char * hex_file)
{
    printf( "\n");
    /*TODO
    printf( "SoftDevice update:\n", hex_file.Nrf51Hex_is_softdevice());
    printf( "    > SD start address: 0x%0.8x\n" % hex_file.sd_address_start);
    printf( "    > SD end address: 0x%0.8x\n" % hex_file.sd_address_end);
    printf( "    > SD size: 0x%0.8x\n" % (hex_file.sd_address_end - hex_file.sd_address_start));
    printf( "Bootloader update:\n", hex_file.Nrf51Hex_is_bootloader());
    printf( "    > BL start address: 0x%0.8x\n" % hex_file.bl_address_start);
    printf( "    > BL end address: 0x%0.8x\n" % hex_file.bl_address_end);
    printf( "    > BL size: 0x%0.8x\n" % (hex_file.bl_address_end - hex_file.bl_address_start));
    printf( "Application update:\n", hex_file.Nrf51Hex_is_application());
    printf( "    > APP start address: 0x%0.8x\n" % hex_file.app_address_start);
    printf( "    > APP end address: 0x%0.8x\n" % hex_file.app_address_end);
    printf( "    > APP size: 0x%0.8x\n" % (hex_file.app_address_end - hex_file.app_address_start));
    */
    printf( "");
}

//def Controller_upload_firmware(self):
        
int Controller_percentage(int part, int whole)
{
    return( (int)(100 * (float)(part)/(float)(whole)) );
}


buf32_t * G_decode_esc_chars(buf32_t * data)
{

    //'''Replace 0xDBDC with 0xCO and 0xDBDD with 0xDB'''
    uint32_t _len;
    uint8_t _char;
    uint8_t _char2;
    static uint8_t resultbuf[1000];
    static buf32_t result32;
    buf32_t *result;
    uint8_t _char_C0;
    uint8_t _char_DB;
    uint32_t pos;

    _char_C0 = 0xC0;
    _char_DB = 0xDB;

    result32.buffer = resultbuf;
    result = &result32;
    buf32_zero(result);

    pos = 0;
    _len = data->length;
    while( pos < _len)
    {
        _char = data->buffer[pos++]; //data.pop(0);
        if(_char == 0xDB)
        {
            _char2 = data->buffer[pos++]; //data.pop(0);
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

int  L_is_timeout(double start_time, double timeout_sec)
{
    //return not (datetime.now() - start_time <= timedelta(0, timeout_sec))
    int rtn;

    rtn = 42;
/*TODO
    if( (datetime.now() - start_time) <= timedelta(0, timeout_sec) ) 
        rtn = 0;
    else
        rtn = 1;
*/
    return(rtn);
}

int Controller_get_ack_nr_NW(Uart_NW_t* uart)
{
    int toCount;
    uint32_t r_serDev;

    uint8_t uart_buffer_buffer[600];
    buf32_t uart_buffer;

    uint8_t temp_buf[600];
    buf32_t temp;

    buf32_t *data;

    //start = datetime.now()

    temp.buffer = temp_buf;

    uart_buffer.buffer = uart_buffer_buffer;
    buf32_zero(&uart_buffer);

    toCount = 0;
    while(buf32_count(&uart_buffer, 0xC0) < 2)
    {
        buf32_zero(&temp);
        //Disregard first of the two C0
        //r_serDev = serdev_Read(uart, temp->buffer, 1, &temp.len); //temp = uart.read(1);
        r_serDev = uart_NW_Read_A(uart, temp.buffer, 1, &temp.length); //temp = uart.read(1);
        if( r_serDev == 0)
        {
            if(temp.length != 0)
            {
                toCount = 0;
                buf32_join(&uart_buffer, &temp); //uart_buffer.append(ord(temp));
            }
            else
            {
                toCount++;
                if(toCount == 10)
                {
                    printf("Controller_get_ack_nr: timeout\n");
                    return(0xFF);
                }
            }
        /*
        if( L_is_timeout(start, 5))
        {
            // reset HciPacket numbering back to 0
            HciPacket.sequence_number = 0;

            // call timeout callback from parent layer
            self.timeout_callback();

            // quit loop
            break;
        }
        */
        }
        else
        {
            printf("Controller_get_ack_nr: serdev_Read: ERROR = %d, 0x%08x\n", r_serDev, r_serDev);
            return(0xFF);
            //break;
        }
    }

    //read until you get a new C0
    //RESUME_WORK
    buf32_print("\nController_get_ack_nr", &uart_buffer);
    data = G_decode_esc_chars(&uart_buffer);

    /*
    // Remove 0xC0 at start and beginning
    data = data[1:-1];
    // print "non-slip data ", [data]
    return (data[0] >> 3) & 0x07;
    */

    // print "non-slip data ", [data]
    return( (data->buffer[1] >> 3) & 0x07);

/*TODO
    //uart_buffer = []
    //start = datetime.now()
    while(uart_buffer.count(0xC0) < 2)
    {
        //Disregard first of the two C0
        temp = uart.read(1);
        if( temp)
            uart_buffer.append(ord(temp));

        if( L_is_timeout(start, 5))
        {
            // reset HciPacket numbering back to 0
            HciPacket.sequence_number = 0;

            // call timeout callback from parent layer
            self.timeout_callback();

            // quit loop
            break
        }
    }
    //read until you get a new C0
    //RESUME_WORK
    data = G_decode_esc_chars(uart_buffer);

    // Remove 0xC0 at start and beginning
    data = data[1:-1];

    // print "non-slip data ", [data]
    return (data[0] >> 3) & 0x07;
*/
}
//https://github.com/NordicSemiconductor/pc-nrfutil/blob/master/nordicsemi/dfu/dfu_transport_serial.py
int Controller_send_packet_NW(Uart_NW_t *uart, buf32_t *pkt)
{
    uint32_t r_serDev;
    uint32_t dwBytesWritten;

    int attempts = 0;
    int ack = 0;//None
    int last_ack = 0;//None
    bool packet_sent = false;
    /* COMMENT FOR NOW*/
    while( packet_sent == false)
    {
      //r_serDev = serdev_Write(uart, pkt->data, pkt->len, &dwBytesWritten);
        r_serDev = uart_NW_Write_A(uart, pkt->buffer, pkt->length, &dwBytesWritten);
        if( r_serDev != 0 )
        {
            printf("Controller_send_packet: serdev_Write: ERROR = %d, 0x%08x\n", r_serDev, r_serDev);
            return(-1);
        }
        if( pkt->length != dwBytesWritten )
        {
            printf("Controller_send_packet: serdev_Write: pkt->len[%d] != dwBytesWritten[%d]\n", pkt->length, dwBytesWritten);
            return(-1);
        }

        attempts += 1;
        ack = Controller_get_ack_nr_NW(uart);
        if( ack == 0xFF)
        {
            printf("Controller_send_packet: Controller_get_ack_nr: ERROR\n");
            return(-1);
        }

        if(last_ack == 0) //None
        {
            last_ack = ack;
            break;
        }
        if( ack == ((last_ack+1) % 8))
        {
            last_ack = ack;
            packet_sent = true;
            if( attempts > 3)
            {
                //raise Exception("Three failed tx attempts encountered on packet {0}".format(seq))
                printf("Three failed tx attempts encountered on packet %d\n", -1 );//.format(seq))
            }
        }
    }
    return(0);

    /*TODO
    while( packet_sent == False)
    {
        uart.write(pkt->buffer)
            attempts += 1
            ack = Controller_get_ack_nr(uart)
            if last_ack == None:
            {
                last_ack = ack
                    break
            }

            if ack == (last_ack+1) % 8:
            {
                last_ack = ack
                    packet_sent = True

                    if attempts > 3:
                    {
                        raise Exception("Three failed tx attempts encountered on packet {0}".format(seq))
                    }
            }
    }
    */
}















uint32_t R2_send_start_dfu( uint32_t mode, uint32_t softdevice_size, uint32_t bootloader_size, uint32_t app_size)
{
    /*
    super(DfuTransportSerial, self).send_start_dfu(mode, softdevice_size, bootloader_size, app_size)

    frame = int32_to_bytes(DFU_START_PACKET)
    frame += int32_to_bytes(mode)
    frame += DfuTransport.create_image_size_packet(softdevice_size, bootloader_size, app_size)

    packet = HciPacket(frame)
    self.send_packet(packet)
    time.sleep(SEND_START_DFU_WAIT_TIME)
    */

    return(42);
}

static uint8_t m_start_data_buffer[600];
static buf32_t m_start_data;

buf32_t* Controller_start_packet_generate(nRFhex_t *pih)
{
    buf32_t* packet = 0;
    uint32_t hex_type;
    
    hex_type = HexType_APPLICATION; //Nrf51Hex_hex_type_get(pih);
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

    malloc_HciPacket_Set_sequence_number( 0 );
    packet = malloc_HciPacket(&m_start_data);
    buf32_print("hci Packet", packet);

    // print "\n\nstart packet data:",
    // for i in packet->buffer:
    //       print hex(ord(i)),
    // print ""
/**/
    return packet;
}


uint32_t R2_send_init_packet(buf32_t *init_packet)
{
/*REF
    frame = int32_to_bytes(DFU_INIT_PACKET);
    frame += int16_to_bytes(hardware_version);    // FF FF 
    frame += int16_to_bytes(hardware_revision);   // FF FF 
    frame += int32_to_bytes(application_version); // FF FF FF FF 
    frame += int16_to_bytes(softdevice_len);      // 01 00 -> 0x0001
    frame += int16_to_bytes(softdevice_array);    // FE FF -> 0xFFFE;
    frame += int16_to_bytes(image_crc);           // 3F 4B -> 0x4B3F;
    frame += int16_to_bytes(0x0000);
*/
    /*
    super(DfuTransportSerial, self).send_init_packet(init_packet)

    frame = int32_to_bytes(DFU_INIT_PACKET)
    frame += init_packet
    frame += int16_to_bytes(0x0000)  # Padding required
    
    packet = HciPacket(frame)
    self.send_packet(packet)
    time.sleep(SEND_INIT_PACKET_WAIT_TIME)
    */
    return(42);
}

buf32_t * Controller_init_packet_generate(uint16_t  image_crc)
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
    packet = malloc_HciPacket(&init_packet);
    buf32_print("init_packet_generate: packet", packet);
    
    return packet;
}


packetList_t* Controller_data_packets_generate(uint8_t *bin_image, uint32_t len)
{
    uint32_t i;
    uint32_t cpy_len;
    uint8_t temp_packet_buffer[600];
    buf32_t temp_packet;
    buf32_t *data_packet;
    static packetList_t data_packets;
    int data_packet_entries;

    temp_packet.buffer = temp_packet_buffer;

    if(data_packets.pktCount != 0)
        free(data_packets.packet);
    data_packets.pktCount = 0;

    data_packet_entries = len / 512;
    if( (len % 512) != 0 )
        data_packet_entries+= 1;

    data_packets.packet = (buf32_t**)calloc(data_packet_entries , sizeof(buf32_t*) );

    for( i=0; i < len; i+= 512)
    {
        cpy_len = len - i;
        if( cpy_len > 512)
            cpy_len = 512;
        buf32_zero(&temp_packet);
        buf32_cpy(&temp_packet, int32_to_bytes(DFU_DATA_PACKET), 4 );
        buf32_cpy(&temp_packet, &bin_image[i], cpy_len );

        //data_packet = malloc_HciPacket(int32_to_bytes(DFU_DATA_PACKET) + bin_image[i:i+512]);
        data_packet = malloc_HciPacket(&temp_packet);

        data_packets.packet[data_packets.pktCount] = data_packet;
        data_packets.pktCount++;
    }
    return(&data_packets);

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


buf32_t * Controller_stop_packet_generate()
{
    buf32_t *packet = 0;
    uint8_t stop_packet_buffer[600];
    buf32_t stop_packet;
    
    stop_packet.buffer = stop_packet_buffer;
    buf32_zero(&stop_packet);
    buf32_cpy(&stop_packet, int32_to_bytes(DFU_END_PACKET), 4); //DFU_STOP_DATA_PACKET

    packet = malloc_HciPacket(&stop_packet);

    // print "\n\nstop packet data:",
    // for i in packet->buffer:
    //       print hex(ord(i)),

    return packet;
}

void hci_dfu_binary_to_packetList_1(void)
{
    int  i;
    int packetCount;

    nRFhex_t *pih;
    uint16_t crc;
    uint16_t bin_image_crc;

    buf32_t *start_packet;
    buf32_t *init_packet;
    buf32_t *stop_packet;
    
    packetList_t *data_packets;

    uint8_t*  pBinImage;
    uint32_t  binImageLength;


    pih = &m_NRFHEX_BOGUS;
    pih->bl_address_start = 0;
    pih->bl_address_end = 0;
    pih->sd_address_start = 0;
    pih->sd_address_end = 0;
    pih->app_address_start = 0x00018000;
    pih->app_address_end = 0x00018000 + (m_gxImage->gx_stop - m_gxImage->gx_start);

    //---------------------------------------------------------------
    //-----
    pBinImage = m_gxImage->gx_bin;
    binImageLength = m_gxImage->gx_len;

    //---------------------------------------------------------------
    //---------------------------------------------------------------
    //-----
    crc = 0xFFFF;
    bin_image_crc = crc16_compute(pBinImage, binImageLength, &crc);
    printf("\n");
    printf("(gx_stop - gx_start) gx_len = %d (0x%x), crc=%04x \n", binImageLength, binImageLength, bin_image_crc);

    packetCount = 0;
    packetCount = binImageLength / 512;
    if( (binImageLength % 512 ) != 0)
        packetCount++;

    packetCount++; // Start Packet
    packetCount++; // Init Packet
    packetCount++; // Stop Packet
    printf("Expected packet count = %d\n", packetCount);

    packetList_CreateEntries(&m_packetList, packetCount);
    //m_packetList.packet = (buf32_t**)calloc(packetCount, sizeof(buf32_t**) );
    //m_packetList.pktCount = 0;


    // Add start packet
    start_packet = Controller_start_packet_generate(pih);
    //TODO m_packetList.append(start_packet);
    m_packetList.packet[m_packetList.pktCount++] = start_packet;

    init_packet = Controller_init_packet_generate(bin_image_crc);
    //TODO m_packetList.append(init_packet);
    m_packetList.packet[m_packetList.pktCount++] = init_packet;
    
    // Add hex data m_packetList
    data_packets = Controller_data_packets_generate(pBinImage, binImageLength);
    //TODO m_packetList.extend(data_packets);
    for( i=0; i<data_packets->pktCount; i++)
        m_packetList.packet[m_packetList.pktCount++] = data_packets->packet[i];

    // Add stop packet
    stop_packet = Controller_stop_packet_generate();
    //TODO m_packetList.append(stop_packet);
    m_packetList.packet[m_packetList.pktCount++] = stop_packet;

    if( gx_Controller_verbose )
        printf("Total number of HCI packets: %i" , m_packetList.pktCount );

}


// see https://github.com/NordicSemiconductor/pc-nrfutil
int hci_dfu_packetList_over_serial_1(void)
{    

    int i;
    int r;
    int Controller_com_port_num;
    int cts_on;

    int32_t uartRet;
    Uart_NW_t  uart0;
    Uart_NW_t* p_uart;

    Controller_com_port_num = 56;
    m_Controller_baud_rate = 38400;
    
    p_uart = &uart0;
    p_uart->hComm = INVALID_HANDLE_VALUE;//(HANDLE)0xFFFFFFFF;

    uartRet = uart_NW_Init_N( p_uart, Controller_com_port_num, m_Controller_baud_rate);//, rtscts=Controller_flow_control, timeout=1);

    uartRet = uart_NW_Open( p_uart );
    if( uartRet != 0 )
    {
        printf("uart_NW_Open could not be opened on %s\n", gx_Controller_com_port);
        return(-1); //??
    }

    //sc__Set0Timeout(p_uart->pserC);

    printf("\n");

    //Sleep(1000);
    //++++++++++++++++++
    do {
        cts_on = uart_NW_IsCTS_ON( p_uart );
        printf("cts_on = %d\n", cts_on);
        if( cts_on == 0 )
            Sleep(10);
    } while( cts_on != 1);

    // Transmit START
    buf32_printShort("Transmit START", m_packetList.packet[0]);
    r = Controller_send_packet_NW(p_uart, m_packetList.packet[0]);
    if( r != 0) goto err;

    Sleep(10000); //SEND_START_DFU_WAIT_TIME 10.0

    //++++++++++++++++++
    do {
        cts_on = uart_NW_IsCTS_ON( p_uart );
        printf("cts_on = %d\n", cts_on);
        if( cts_on == 0 )
            Sleep(10);
    } while( cts_on != 1);

    // Transmit INIT
    buf32_printShort("Transmit INIT", m_packetList.packet[1]);
    r = Controller_send_packet_NW(p_uart, m_packetList.packet[1]);
    if( r != 0) goto err;
    Sleep(1000); //SEND_INIT_PACKET_WAIT_TIME 1.0


    // Transmit DATA
    for( i = 2 ; i < m_packetList.pktCount ; i++ )
    {
        //++++++++++++++++++
        do {
            cts_on = uart_NW_IsCTS_ON( p_uart );
            printf("cts_on = %d\n", cts_on);
            if( cts_on == 0 )
                Sleep(10);
        } while( cts_on != 1);

        buf32_printShort("Transmit DATA etc", m_packetList.packet[i]);
        r = Controller_send_packet_NW(p_uart, m_packetList.packet[i]);
        if( r != 0) goto err;
        Controller_progress_callback( Controller_percentage(i, m_packetList.pktCount), 30 );
    }
    
    /*TODO
    Sleep(2);
    uart.write(m_packetList[-1]->buffer);
    */
    Controller_progress_callback( Controller_percentage(m_packetList.pktCount, m_packetList.pktCount), 30 );
    printf("\n");
    printf("\n");
    printf("Success!\n");
    printf("\n");

  err:
    uartRet = uart_NW_Close(p_uart); //uart.close()

    return(0);
}


extern "C" void Controller_upload_firmware_1(void);
void Controller_upload_firmware_1(void)
{
    int32_t r;


    m_gxImage = &m_gxImage0;
    m_gxImage->gx_bin = m_gxImage_buffer;
    m_gxImage->gx_binCapacity = GXIMAGE_BUFFER_CAPACITY; //OR  m_gxImage->gx_binCapacity = sizeof(m_gxImage_buffer);    
    r = W_ihexLoadToMem( m_gxImage, "C:/ble_nrf51/tandd/nrf51/examples/ble_peripheral/ble_app_K2/pca10028/s110/arm4/_build/nrf51422_xxac_s110.hex" );
    if( r != 0)
    {
        return;
    }

    //-----------------------------------------------------
    hci_dfu_binary_to_packetList_1();


    //=====================================================
    //-----------------------------------------------------
    /* comment for testing*/
    hci_dfu_packetList_over_serial_1();
    /**/


    int i;
    for( i = 0 ; i < m_packetList.pktCount ; i++ )
    {
        buf32_Destroy( &m_packetList.packet[i]);
    }
    
    packetList_DestroyEntries(m_packetList);


}



