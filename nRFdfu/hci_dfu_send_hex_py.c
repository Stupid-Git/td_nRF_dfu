#include "stdint.h"
#include "stdio.h"


/*
import sys, os
import time
import intelhex
from ctypes import c_uint32, c_uint8
from threading import Thread
from serial import Serial
from datetime import datetime, timedelta
import argparse
import crc16pure

VERSION = '1.0'


import getopt
*/

/*
def convert_uint32_to_array(val):
    uint16_data = c_uint32(val)
    byte0 = c_uint8((uint16_data.value & 0x000000FF))
    byte1 = c_uint8((uint16_data.value & 0x0000FF00) >> 8)
    byte2 = c_uint8((uint16_data.value & 0x00FF0000) >> 16)
    byte3 = c_uint8((uint16_data.value & 0xFF000000) >> 24)
    
    return [byte0.value, byte1.value, byte2.value, byte3.value]
*/
/*TODO
uint32_t convert_uint32_to_array(val)
{
    uint16_data = c_uint32(val)
    byte0 = c_uint8((uint16_data.value & 0x000000FF))
    byte1 = c_uint8((uint16_data.value & 0x0000FF00) >> 8)
    byte2 = c_uint8((uint16_data.value & 0x00FF0000) >> 16)
    byte3 = c_uint8((uint16_data.value & 0xFF000000) >> 24)
    
    return [byte0.value, byte1.value, byte2.value, byte3.value]
}
*/

/*TTOO
class ThreadedFunction(Thread):
    
    def __init__(self, func, *args, **kwargs):
        self.func = func
        self.args = args 
        self.kwargs = kwargs
        super(ThreadedFunction, self).__init__()
    
    def run(self):
        self.func(*self.args, **self.kwargs)
*/
//class HexType:
#define  SOFTDEVICE   1
#define  BOOTLOADER   2
#define  SD_BL        3
#define  APPLICATION  4

/*
class Nrf51Hex(intelhex.IntelHex):
       
    def Nrf51Hex__init__(self, *args, **kwargs):
        super(Nrf51Hex, self).__init__(*args, **kwargs)
        self.sd_address_start = 0
        self.sd_address_end = 0
        self.bl_address_start = 0
        self.bl_address_end = 0
        self.app_address_start = 0
        self.app_address_end = 0

        if self.Nrf51Hex_is_softdevice():
            self.sd_address_start = 4096
            self.sd_address_end = int(self.gets(0x3008, 4)[::-1].encode('hex'), 16)

        if self.Nrf51Hex_is_bootloader():
            self.bl_address_start = self._resolve_start_address(0x30000, 0x40000)
            self.bl_address_end = self._resolve_end_address(0x30000, 0x40000)
            
        if self.Nrf51Hex_is_application():
            self.app_address_start = self.minaddr()
            self.app_address_end = self.maxaddr() + 1

    def Nrf51Hex_hex_type_get(self):
        if self.Nrf51Hex_is_softdevice() and self.Nrf51Hex_is_bootloader():
            return HexType.SD_BL

        if self.Nrf51Hex_is_softdevice():
            return HexType.SOFTDEVICE

        if self.Nrf51Hex_is_bootloader():
            return HexType.BOOTLOADER

        if self.Nrf51Hex_is_application():
            return HexType.APPLICATION

    def Nrf51Hex_hex_info(hexfile):
        start = hexfile.minaddr()
        end   = hexfile.maxaddr() + 1
        size  = end - start
        return size, start, end

    def Nrf51Hex_is_softdevice(self):
        inside_boundry = self.minaddr() < 0x1004

        return True if (inside_boundry and self._has_sp(4096)) else False

    def Nrf51Hex_is_application(self):
        inside_boundry = self.minaddr() > 0x13FFC and self.minaddr() < 0x30000

        return True if (inside_boundry and self._has_sp(self.minaddr())) else False

    def Nrf51Hex__resolve_start_address(self, lower_limit, upper_limit):
        test_address = lower_limit
        while test_address < upper_limit:
            try:
                has_sp = self._has_sp(test_address)
                if has_sp:
                    break
            except intelhex.NotEnoughDataError, e:
                test_address += 0x400
        
        // Return the found start address if this is less than the upper limit
        // else return None to indicate nothing was found        
        return test_address if test_address < upper_limit else None

    def Nrf51Hex__resolve_end_address(self, lower_limit, upper_limit):
        // find the last word in the bootloader, by going from the back until code is hit.
        end_address = upper_limit
        while upper_limit >= lower_limit:
            try:
                data = self.gets(end_address, 4)
                end_address = end_address + 0x04
                break

            except intelhex.NotEnoughDataError, e:
                end_address = end_address - 0x04

        return end_address

    def Nrf51Hex_is_bootloader(self):
        inside_boundry = self.maxaddr() > 0x30000 
        has_sp = False

        addr_start = 0x30000
        addr_end = 0x40000

        start_address = self._resolve_start_address(addr_start, addr_end)

        // if not None, we have a valid SP in the start address
        if start_address:
            has_sp = True

        return True if (inside_boundry and has_sp) else False

    def Nrf51Hex__has_sp(self, sp_address):
        little_endian_sp_address = self.gets(sp_address, 4) 
        init_sp = int(little_endian_sp_address[::-1].encode('hex'), 16)
        //print "%x" % init_sp

        return True if (init_sp > 0x20000000) else False


def G_open_hex(filename):
    try:
        ih = intelhex.IntelHex(filename)
        return ih
    except (IOError, intelhex.IntelHexError), e:
        print ('Error reading file: %s\n' % e)
        raise Exception("Could not read hex format")

*/

typedef struct _buf600
{
    uint32_t len;
    uint8_t data[600];
} buf6_t;

buf6_t gx_ret_buf600;


void buf6_zero(buf6_t *P)
{
    P->len = 0;
}

#include "memory.h"
void buf6_join(buf6_t *P, buf6_t *P2)
{
    memcpy(&P->data[P->len], &P2->data[P2->len], P2->len);
    P->len += P2->len;
}

buf6_t* G_parts_to_four_bytes(uint32_t seq, uint32_t dicp, uint32_t rel, uint32_t pkt_type, uint32_t pkt_len)
{
    uint8_t ints[4];

    ints[0] = 0;  ints[1] = 0;  ints[2] = 0;  ints[3] = 0;

    ints[0] = (seq) | (((seq+1)%8) << 3) | (dicp << 6) | (rel << 7);
    ints[1] = pkt_type | ((pkt_len & 0x000F) << 4);
    ints[2] = (pkt_len & 0x0FF0) >> 4;
    ints[3] = ( ~(ints[0]+ints[1]+ints[2]+ints[3]) + 1 ) & 0xFF;

    buf6_zero(&gx_ret_buf600);
    gx_ret_buf600.data[0] = ints[0];
    gx_ret_buf600.data[1] = ints[1];
    gx_ret_buf600.data[2] = ints[2];
    gx_ret_buf600.data[3] = ints[3];
    gx_ret_buf600.len = 4;
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

uint8_t * int32_to_bytes(uint32_t nr)
{
    static uint8_t ints[4];
    ints[0] = (nr & 0x000000FF) ;
    ints[1] = (nr & 0x0000FF00) >> 8;
    ints[2] = (nr & 0x00FF0000) >> 16;
    ints[3] = (nr & 0xFF000000) >> 24;
    return(ints);
    //return ''.join(chr(b) for b in ints)
}


    /*
def G_int16_to_bytes(nr):
    ints = [0,0]
    ints[0] = (nr & 0x00FF) 
    ints[1] = (nr & 0xFF00) >> 8
    return ''.join(chr(b) for b in ints)     

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
TTOO*/ 

buf6_t * G_encode_packet(buf6_t *data_in)
{
    // Replace 0xCO  with 0xDBDC and 0xDB with 0xDBDD
    static buf6_t result;
    int i;
    int j;
    uint8_t _char;

    buf6_zero(&result);
    j=0;

    for( i=0; i<data_in->len; i++)
    {
        _char = data_in->data[i];
        if(_char == 0xC0)
        {
            result.data[j++] = 0xDB;
            result.data[j++] = 0xDC;
        }
        else
            if(_char == 0xDB)
            {
                result.data[j++] = 0xDB;
                result.data[j++] = 0xDD;
            }
            else
            {
                result.data[j++] = _char;
            }
    }
    result.len = j;
    return(&result);

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

#define DATA_INTEGRITY_CHECK_PRESENT   1
#define RELIABLE_PACKET    1
#define HCI_PACKET_TYPE   14

#define DFU_INIT_PACKET     1
#define DFU_START_PACKET    3
#define DFU_DATA_PACKET     4
#define DFU_END_PACKET      5

#define DFU_UPDATE_MODE_NONE    0
#define DFU_UPDATE_MODE_SD      1
#define DFU_UPDATE_MODE_BL      2
#define DFU_UPDATE_MODE_APP     4



struct _HciPacket
{
    uint32_t sequence_number;//TODO = 0;
    //buf6_t temp_data;
} HciPacket;

//#include "memory.h"
#include "stdlib.h" //malloc
buf6_t* new_HciPacket(buf6_t* p_data_in)
{
    uint16_t crc;
    buf6_t * p_buff600;
    buf6_t *p_self_data;
    static buf6_t self_temp_data;
    
    p_self_data = (buf6_t*)malloc(sizeof(buf6_t));

    HciPacket.sequence_number = (HciPacket.sequence_number+1) % 8;
    buf6_zero(&self_temp_data);
    buf6_join(&self_temp_data, G_parts_to_four_bytes(HciPacket.sequence_number, 
                                        DATA_INTEGRITY_CHECK_PRESENT,
                                        RELIABLE_PACKET,
                                        HCI_PACKET_TYPE,
                                        p_data_in->len) ); //self_data.len) ); //len(data))    );
    
    buf6_join(&self_temp_data, p_data_in); //&self_data);
    //// Add escape caracters
    crc = crc16pure.crc16xmodem(&self_temp_data, crc=0xFFFF);

    self_temp_data.data[self_temp_data.len] = (uint8_t)(crc & 0xFF); self_temp_data.len++; //chr(crc & 0xFF)
    self_temp_data.data[self_temp_data.len] = (uint8_t)((crc & 0xFF00) >> 8); self_temp_data.len++; //chr((crc & 0xFF00) >> 8)
               
    p_buff600 = G_encode_packet(&self_temp_data);
        
    //buf6_zero(&self_data);    
    p_self_data->len = 0;
    p_self_data->data[p_self_data->len] = 0xC0;  //self.data = chr(0xc0)
    p_self_data->len++;

    buf6_join(p_self_data, p_buff600);             //self.data += self.temp_data
    
    p_self_data->data[p_self_data->len] = 0xC0;  //self.data += chr(0xc0)
    p_self_data->len++;
    return(p_self_data);
}
     

//class Controller(object):
char *Controller_file_path = 0;
char *Controller_com_port = 0;//None;
int   Controller_flow_control = 0;//None;
int   Controller_baud_rate = 0;//None;
int   Controller_verbose = 0; //None;
int   Controller_progress_value = -1;
void Controller_init()
{
    Controller_file_path = 0;
    Controller_com_port = 0;//None;
    Controller_flow_control = 0;//None;
    Controller_baud_rate = 0;//None;
    Controller_verbose = 0;//None;
    Controller_progress_value = -1;
}        

void Controller_progress_callback(int value, int bar_length/*=30*/)
{
    if(value != Controller_progress_value)
    {
        Controller_progress_value = value;
        /*TODO
        hashes = '//' * int((float(Controller_progress_value)/100) * bar_length)
        spaces = ' ' * (bar_length - len(hashes))
        sys.stdout.write("\rProgress: [{0}] {1}%".format(hashes + spaces, Controller_progress_value))
        sys.stdout.flush()
        */
        printf("Progress: %d\n", Controller_progress_value );
    }
}

void Controller_timeout_callback()
{
    printf("Transmission timeout.\n");
}

void Controller_set_filepath(char* filepath)
{
    Controller_file_path = filepath;
    /*TODO
    if not os.path.isfile(os.path.abspath(Controller_file_path))
    {
        print "File does not exist: %s" % os.path.abspath(Controller_file_path)
            sys.exit(1)
    }
    */
}
      
void Controller_upload_file()
{
    /*TODO
    if(Controller_file_path == 0)
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
    return int(100 * float(part)/float(whole));
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

int Controller_get_ack_nr(void* uart)
{

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
    return(42);
}

//https://github.com/NordicSemiconductor/pc-nrfutil/blob/master/nordicsemi/dfu/dfu_transport_serial.py
int Controller_send_packet(void *uart, buf6_t *pkt)
{
    int attempts = 0;
    int last_ack = 0;//None
    bool packet_sent = false;
    /*TODO
    while( packet_sent == False)
    {
        uart.write(pkt.data)
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
    return(42);
}


buf6_t * Controller_start_packet_generate(int64_t hex_file)
{
    buf6_t *packet = 0;
/*TODO
    //def word_align(a, b):
    //    return (((a) + (b - 1)) &~(b - 1))

    hex_type = hex_file.Nrf51Hex_hex_type_get();

    start_data = int32_to_bytes(DFU_START_PACKET);
    start_data += int32_to_bytes(hex_type);
    // size of SoftDevice
    start_data += int32_to_bytes(word_align(hex_file.sd_address_end, 4) - hex_file.sd_address_start) ;
    // size of BootLoader
    start_data += int32_to_bytes(word_align(hex_file.bl_address_end, 4) - hex_file.bl_address_start) ;
    // size of Application
    start_data += int32_to_bytes(word_align(hex_file.app_address_end, 4) - hex_file.app_address_start) ;

    packet = new_HciPacket(start_data);

    // print "\n\nstart packet data:",
    // for i in packet.data:
    //       print hex(ord(i)),
    // print ""
*/
    return packet
}

buf6_t * Controller_init_packet_generate(uint16_t  image_crc)
{
    buf6_t *packet = 0;
    buf6_t init_packet;

    uint16_t hardware_version    = 0xFFFF;
    uint16_t hardware_revision   = 0xFFFF;
    uint32_t application_version = 0xFFFFFFFF;
    uint16_t softdevice_len      = 0x0001;
    uint16_t softdevice_array    = 0xFFFE;

/*TODO
    init_packet = int32_to_bytes(DFU_INIT_PACKET);
    init_packet +=  int16_to_bytes(hardware_version);
    init_packet += int16_to_bytes(hardware_revision);
    init_packet += int32_to_bytes(application_version);
    init_packet += int16_to_bytes(softdevice_len);
    init_packet += int16_to_bytes(softdevice_array);
    init_packet += int16_to_bytes(image_crc);
    init_packet += int16_to_bytes(0x0000);
*/
    packet = new_HciPacket(&init_packet);
    return packet;
}


buf6_t * Controller_stop_packet_generate()
{
    buf6_t *packet = 0;
    buf6_t *stop_data = 0;

    /*TODO
    stop_data = int32_to_bytes(DFU_END_PACKET);
    */
    packet = new_HciPacket(stop_data);

    // print "\n\nstop packet data:",
    // for i in packet.data:
    //       print hex(ord(i)),

    return packet;
}


void * Controller_data_packets_generate(uint8_t *bin_image)
{
    //def word_align(a, b):
    //    return (((a) + (b - 1)) &~(b - 1))
    //foo_packets = []  
    /*TODO
    for i in range(0, len(bin_image), 512)
    {
        data_packet = new_HciPacket(int32_to_bytes(DFU_DATA_PACKET) + bin_image[i:i+512])

            // print "\n\ndata packet data:",
            // for i in data_packet.data:
            //     print hex(ord(i)),

            foo_packets.append(data_packet)
    }
    return foo_packets;
    */
    return((void*)42);
}

void Controller_upload_firmware()
{
        //self.test_sd()

        ih = Nrf51Hex(Controller_file_path);

        // if verbose flag is set.
        if( Controller_verbose)
            Controller_verbose_info(ih);

        //packets = []

        char *bin_image = "";

        if ih.Nrf51Hex_is_softdevice():
            bin_image += ih.tobinstr(start = ih.sd_address_start, end = ih.sd_address_end - 1) // remove last address as intelhex module includes the end address.
        if ih.Nrf51Hex_is_bootloader():
            bin_image += ih.tobinstr(start = ih.bl_address_start, end = ih.bl_address_end - 1)
        if ih.Nrf51Hex_is_application():
            bin_image += ih.tobinstr(start = ih.app_address_start, end = ih.app_address_end - 1)

        bin_image_crc = crc16pure.crc16xmodem(bin_image, crc=0xFFFF)
        
        // Add start packet
        start_packet = Controller_start_packet_generate(ih)
        packets.append(start_packet)

        init_packet = Controller_init_packet_generate(bin_image_crc)
        packets.append(init_packet)

        // Add hex data packets
        data_packets = Controller_data_packets_generate(bin_image)
        packets.extend(data_packets)
        
        // Add stop packet
        stop_packet = Controller_stop_packet_generate()
        packets.append(stop_packet)
        
        if Controller_verbose:
            print "Total number of HCI packets: %i" % len(packets)

        uart = None
        
        try:
            uart = Serial(Controller_com_port, Controller_baud_rate, rtscts=Controller_flow_control, timeout=1)
        except Exception, e:
            print "UART could not be opened on %s" % Controller_com_port
            print e 
        try:
            if uart:
                print ""
                // Transmit START
                Controller_send_packet(uart, packets[0])
                time.sleep(10)

                // Transmit INIT
                Controller_send_packet(uart, packets[1])

                // Transmit DATA
                for seq, pkt in enumerate(packets[2:-1]):
                    Controller_send_packet(uart, pkt)
                    Controller_progress_callback(Controller_percentage(seq, len(packets)))
                time.sleep(2)
                uart.write(packets[-1].data)
                Controller_progress_callback(Controller_percentage(len(packets), len(packets)))
                print ""
                print ""
                print "Success!"
                print ""
                uart.close()

        except IndexError, e:
            print "Ack out of sequence, or no ack returned"
} //Controller_upload_firmware        

int mmm_main()
{

    //def main(argv,arglen):    
    //    print ("")
           
/*TODO

if __name__ == '__main__':
    
    controller = Controller()
    
    parser = argparse.ArgumentParser(description='Hex File of firmware to be upgraded.')
    parser.add_argument('--file', '-f',
                        type=str,
                        required=True,
                        dest='file',
                        help='Filename of Hex file.')
    parser.add_argument('--port', '-p',
                        type=str,
                        required=True,
                        dest='comport',
                        help='COM Port to which the device is connected.')
    parser.add_argument('--flowcontrol', '-fc',
                        action='store_true',
                        required=False,
                        dest='flowcontrol_bool',
                        help='Enable flow control, default: disabled.')
    parser.add_argument('--baudrate', '-b',
                        type=int,
                        required=False,
                        default=38400,
                        dest='baudrate',
                        help='Desired baudrate 38400/96000/115200/230400/250000/460800/921600/1000000 (default: 38400). Note: Baud rates >115200 are supported by nRF51822, but may not be supported by all RS232 devices on windows.')
    parser.add_argument('--verbose', '-v',
                        action='store_true',
                        default=False,
                        dest='verbose',
                        help='Enable verbose mode.')


    args = parser.parse_args()
    print 'Sending file {0} to {1}, flow control = {2}'.format(args.file, args.comport, args.flowcontrol_bool)
    
    controller.Controller_com_port = args.comport.strip()
    controller.Controller_flow_control = args.flowcontrol_bool
    controller.Controller_baud_rate = args.baudrate 
    controller.Controller_verbose = args.verbose
    
    controller.Controller_set_filepath(args.file.strip())
    */
    /*controller.*/Controller_upload_file();

    return(0);
}

